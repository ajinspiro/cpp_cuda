////////////////////////////////////////////////////
//
// A Simple Java Swing based Clock Program
//
// Converted from the MFC version originally written by Praseed Pai K.T.
//              http://praseedp.blogspot.com
//
////////////////////////////////////////////////////

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.time.LocalTime;

public class App {
    static final double PI = 3.14159;

    //////////////////////////
    //
    // The Clock Panel (equivalent of CClockFrame's OnPaint / OnCreate / OnTimer)
    //
    //////////////////////////
    static class ClockPanel extends JPanel implements ActionListener {

        public ClockPanel() {
            // Background color RGB(175, 238, 238) -- pale turquoise
            setBackground(new Color(175, 238, 238));

            // Timer that fires every 1000 ms, like SetTimer(1, 1000, NULL)
            Timer timer = new Timer(1000, this);
            timer.start();
        }

        // Equivalent of OnTimer -> Invalidate()
        @Override
        public void actionPerformed(ActionEvent e) {
            repaint();
        }

        // Equivalent of Transform(int*, int*)
        // Converts "math" coordinates (origin at center, y-up) into
        // Swing/AWT screen coordinates (origin top-left, y-down).
        private Point transform(int x, int y) {
            int width = getWidth() / 2;
            int height = getHeight() / 2;
            int px = x + width;
            int py = height - y;
            return new Point(px, py);
        }

        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            Graphics2D d = (Graphics2D) g;
            d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

            // ----- Draw the clock face rectangle -----
            // CBrush b(RGB(100, 149, 237)); (cornflower blue)
            int x1 = -200;
            int y1 = -220;
            int x2 = 210;
            int y2 = 200;
            Point p1 = transform(x1, y1);
            Point p2 = transform(x2, y2);

            int rx = Math.min(p1.x, p2.x);
            int ry = Math.min(p1.y, p2.y);
            int rw = Math.abs(p2.x - p1.x);
            int rh = Math.abs(p2.y - p1.y);

            d.setColor(new Color(100, 149, 237));
            d.fillRect(rx, ry, rw, rh);

            // ----- Set up the pen for the hands -----
            // CPen p2(PS_SOLID, 2, RGB(153, 0, 0));
            d.setColor(new Color(153, 0, 0));
            d.setStroke(new BasicStroke(2));

            // ----- Compute hand angles from the current time -----
            LocalTime t = LocalTime.now();

            int fhour = t.getHour() % 12;
            int fmin = t.getMinute();
            int fsec = t.getSecond();

            // NOTE: ported as-is from the original MFC code, including the
            // (mostly no-op due to integer division) fhour += fmin / 60 step.
            fhour += fmin / 60;
            fhour = fhour * 360 / 12;
            fmin = fmin * 360 / 60;
            fsec = fsec * 360 / 60;

            int xs = (int) (120 * Math.cos((-fsec * PI / 180.0) + PI / 2.0));
            int ys = (int) (120 * Math.sin((-fsec * PI / 180.0) + PI / 2.0));

            int xm = (int) (100 * Math.cos((-fmin * PI / 180.0) + PI / 2.0));
            int ym = (int) (100 * Math.sin((-fmin * PI / 180.0) + PI / 2.0));

            int xh = (int) (60 * Math.cos((-fhour * PI / 180.0) + PI / 2.0));
            int yh = (int) (60 * Math.sin((-fhour * PI / 180.0) + PI / 2.0));

            int cx = 0;
            int cy = 0;

            Point center = transform(cx, cy);

            // Hour hand
            Point hourPt = transform(xh, yh);
            d.drawLine(center.x, center.y, hourPt.x, hourPt.y);

            // Minute hand
            Point minPt = transform(xm, ym);
            d.drawLine(center.x, center.y, minPt.x, minPt.y);

            // Second hand
            Point secPt = transform(xs, ys);
            d.drawLine(center.x, center.y, secPt.x, secPt.y);

            // ----- Draw the hour numbers around the face -----
            String[] str = { "3", "2", "1", "12", "11", "10", "9", "8", "7", "6", "5", "4" };

            Font f = new Font("Times New Roman", Font.PLAIN, 16);
            d.setFont(f);
            d.setColor(new Color(204, 0, 0));

            FontMetrics fm = d.getFontMetrics();

            for (int i = 0; i < 12; i++) {
                int nx = (int) (200 * Math.cos(i * PI / 6));
                int ny = (int) (200 * Math.sin(i * PI / 6));
                Point np = transform(nx, ny);

                // TextOut draws from the top-left corner of the text;
                // drawString uses the baseline, so shift up by the ascent.
                d.drawString(str[i], np.x, np.y + fm.getAscent() / 2 - 2);
            }
        }
    }

    ////////////////////////////////////
    // Our Application Object
    // (equivalent of CClockApp / CClockFrame construction)
    ////////////////////////////////////
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            JFrame frame = new JFrame("MFC Clock By Praseed Pai");
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.setSize(500, 500);
            frame.setLocationRelativeTo(null);
            frame.add(new ClockPanel());
            frame.setVisible(true);
        });
    }
}
