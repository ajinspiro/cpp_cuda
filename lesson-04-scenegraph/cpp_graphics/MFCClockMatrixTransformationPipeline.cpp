////////////////////////////////////////////////////
//
// A Simple MFC based Clock Program
// -- rewritten to use a 2D homogeneous-coordinate
//    matrix transformation pipeline --
//
// Original by Praseed Pai K.T.
//              http://praseedp.blogspot.com
//
// The original code hand-transformed every point with a
// one-off Transform(int*, int*) function that translated to
// the window center and flipped the Y axis. Here, that
// transform is expressed as a proper matrix (Translation *
// Scale), and each clock hand / numeral is positioned by
// composing a Rotation matrix with that same view matrix:
//
//      screenPoint = ViewMatrix * RotationMatrix * modelPoint
//
// Rotation is applied first (in "model space", where the
// clock face is centered at the origin with Y pointing up),
// then the combined result is pushed into device/window
// coordinates by the view matrix. This is the same pipeline
// idea used in real 2D/3D graphics APIs (model -> world ->
// view -> screen), just written out by hand with a small
// Matrix3 class instead of relying on a graphics library.
//
////////////////////////////////////////////////////
#include <afxwin.h>
#include <afxext.h>
#include <math.h>

//////////////////////////////////
//
// constant to represent PI
//
//////////////////////////////////
const double PI = 3.14159265358979323846;

//////////////////////////////////////////////////////////
//
// Matrix3
//
// A 3x3 matrix operating on 2D points in homogeneous
// coordinates (x, y, 1). Supports the three primitive
// transforms we need (identity, translation, scale,
// rotation) plus composition via operator*, and applying
// a matrix to a point.
//
// Convention: column vectors, so (A * B) means "apply B
// first, then A" -- exactly like function composition.
//
//////////////////////////////////////////////////////////
struct Matrix3
{
    double m[3][3];

    static Matrix3 Identity()
    {
        Matrix3 r = {};
        r.m[0][0] = 1.0;
        r.m[1][1] = 1.0;
        r.m[2][2] = 1.0;
        return r;
    }

    static Matrix3 Translation(double tx, double ty)
    {
        Matrix3 r = Identity();
        r.m[0][2] = tx;
        r.m[1][2] = ty;
        return r;
    }

    static Matrix3 Scale(double sx, double sy)
    {
        Matrix3 r = Identity();
        r.m[0][0] = sx;
        r.m[1][1] = sy;
        return r;
    }

    // Standard counter-clockwise rotation by `radians`.
    static Matrix3 Rotation(double radians)
    {
        Matrix3 r = Identity();
        double c = cos(radians);
        double s = sin(radians);
        r.m[0][0] = c;  r.m[0][1] = -s;
        r.m[1][0] = s;  r.m[1][1] = c;
        return r;
    }

    // Compose: (*this) * rhs => apply rhs first, then *this.
    Matrix3 operator*(const Matrix3 &rhs) const
    {
        Matrix3 result = {};
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                double sum = 0.0;
                for (int k = 0; k < 3; k++)
                    sum += m[i][k] * rhs.m[k][j];
                result.m[i][j] = sum;
            }
        }
        return result;
    }

    // Apply this matrix to a homogeneous point (x, y, 1)
    // and return the resulting device-space point.
    CPoint Apply(double x, double y) const
    {
        double px = m[0][0] * x + m[0][1] * y + m[0][2];
        double py = m[1][0] * x + m[1][1] * y + m[1][2];
        return CPoint(RoundToInt(px), RoundToInt(py));
    }

    static int RoundToInt(double v)
    {
        return (int)(v >= 0.0 ? v + 0.5 : v - 0.5);
    }
};

/////////////////////////////
//
// The Main Frame Window
//
/////////////////////////////
class CClockFrame : public CFrameWnd
{
public:
    CClockFrame()
    {
        HBRUSH brush = (HBRUSH)::CreateSolidBrush(RGB(175, 238, 238));
        CString mywindow = AfxRegisterWndClass(
            CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 0, brush, 0);

        Create(mywindow, _T("MFC Clock (Matrix Pipeline) By Praseed Pai"));
    }

    int OnCreate(LPCREATESTRUCT l)
    {
        CFrameWnd::OnCreate(l);
        SetTimer(1, 1000, NULL);
        return 1;
    }

    // Builds the "view" matrix: moves the origin to the
    // center of the client area and flips the Y axis, since
    // our model space has Y pointing up but window
    // coordinates increase downward.
    Matrix3 BuildViewMatrix()
    {
        CRect rc;
        GetClientRect(&rc);
        double halfW = rc.Width() / 2.0;
        double halfH = rc.Height() / 2.0;
        return Matrix3::Translation(halfW, halfH) * Matrix3::Scale(1.0, -1.0);
    }

    // Draws one clock hand as a line from the center to a
    // point `length` units "up" in model space, rotated by
    // `angleDegrees` (clockwise, 0 = 12 o'clock), then
    // pushed through the view matrix.
    void DrawHand(CDC &d, const Matrix3 &view, double angleDegrees, double length)
    {
        double theta = -angleDegrees * PI / 180.0;
        Matrix3 handMatrix = view * Matrix3::Rotation(theta);

        CPoint origin = view.Apply(0, 0);
        CPoint tip = handMatrix.Apply(0, length);

        d.MoveTo(origin.x, origin.y);
        d.LineTo(tip.x, tip.y);
    }

    void OnPaint()
    {
        CPaintDC d(this);
        CBrush faceBrush(RGB(100, 149, 237));

        Matrix3 view = BuildViewMatrix();

        // ----- Clock face -----
        CPoint p1 = view.Apply(-200, -220);
        CPoint p2 = view.Apply(210, 200);
        CRect faceRect(p1, p2);
        faceRect.NormalizeRect();
        d.FillRect(&faceRect, &faceBrush);

        CPen handPen(PS_SOLID, 2, RGB(153, 0, 0));
        d.SelectObject(&handPen);

        // ----- Compute hand angles (0..360) from the time -----
        CTime t = CTime::GetCurrentTime();

        int fhour = t.GetHour() % 12;
        int fmin = t.GetMinute();
        int fsec = t.GetSecond();

        fhour += fmin / 60;          // kept as in the original
        fhour = fhour * 360 / 12;
        fmin = fmin * 360 / 60;
        fsec = fsec * 360 / 60;

        // ----- Draw the hands: each is just a rotation matrix
        //       composed with the view matrix -----
        DrawHand(d, view, fhour, 60);   // hour hand
        DrawHand(d, view, fmin, 100);   // minute hand
        DrawHand(d, view, fsec, 120);   // second hand

        // ----- Hour numerals -----
        const char *str[] = {"3", "2", "1", "12", "11", "10", "9", "8", "7", "6", "5", "4"};
        CFont f;
        f.CreatePointFont(120, "Times New Roman");
        d.SelectObject(&f);
        d.SetTextColor(RGB(204, 0, 0));
        d.SetBkMode(TRANSPARENT);

        for (int i = 0; i < 12; i++)
        {
            double theta = i * PI / 6.0;
            Matrix3 numMatrix = view * Matrix3::Rotation(theta);
            CPoint p = numMatrix.Apply(200, 0);
            d.TextOut(p.x, p.y, CString(str[i]), strlen(str[i]));
        }
    }

    void OnTimer(UINT_PTR n)
    {
        this->Invalidate(0);
    }

    DECLARE_MESSAGE_MAP();
};

BEGIN_MESSAGE_MAP(CClockFrame, CFrameWnd)
ON_WM_CREATE()
ON_WM_PAINT()
ON_WM_TIMER()
END_MESSAGE_MAP()

///////////////////////////////////////
// Our Application Object
///////////////////////////////////////
class CClockApp : public CWinApp
{
public:
    int InitInstance()
    {
        CClockFrame *p;
        p = new CClockFrame();
        p->ShowWindow(1);
        m_pMainWnd = p;
        return 1;
    }
};

/////////////////////////
//
// Every Application requires a
// Global Application Object instance
//
/////////////////////////
CClockApp a;

// Eod ClockMatrixPipeline.cpp