import java.io.*;
import java.math.*;
import java.util.*;

public class timus1951 {
    static BufferedReader in;
    static PrintWriter out;

    public static void main(String[] args) throws Exception {
        in = new BufferedReader(new InputStreamReader(System.in));
        out = new PrintWriter(System.out);
        solve();
        out.close();
    }

    static class Complex {
        BigInteger re;
        BigInteger im;

        Complex(BigInteger re, BigInteger im) {
            this.re = re;
            this.im = im;
        }

        Complex mul(Complex other) {
            BigInteger real = re.multiply(other.re).subtract(im.multiply(other.im));
            BigInteger imag = re.multiply(other.im).add(im.multiply(other.re));
            return new Complex(real, imag);
        }

        Complex pow(int n) {
            Complex result = new Complex(BigInteger.ONE, BigInteger.ZERO);
            Complex base = this;
            while (n > 0) {
                if ((n & 1) == 1)
                    result = result.mul(base);
                base = base.mul(base);
                n >>= 1;
            }
            return result;
        }

        boolean equals(Complex other) {
            return re.equals(other.re) && im.equals(other.im);
        }
    }

    public static void solve() throws Exception {
        StringTokenizer st = new StringTokenizer(in.readLine());
        Complex a = new Complex(new BigInteger(st.nextToken()), new BigInteger(st.nextToken()));
        int n = Integer.parseInt(st.nextToken());

        st = new StringTokenizer(in.readLine());
        Complex b = new Complex(new BigInteger(st.nextToken()), new BigInteger(st.nextToken()));
        int m = Integer.parseInt(st.nextToken());

        Complex am = a.pow(m);
        Complex bn = b.pow(n);

        if (!am.equals(bn)) {
            out.println(0);
            return;
        }

        int fullDiff = findFull(a, m, b, n);

        int result = 0;
        for (int i = 0; i < n; i++) {
          int circles = fullDiff + m * i;
          boolean found = false;
          for (int j = 0; j < m; j++) {
            if ((circles - n * j) % (m * n) == 0)
              found = true;
          }
          if (found)
            result++;
        }
        out.println(result);
    }

    public static int findFull(Complex a, int m, Complex b, int n) {
      double angleA = Math.atan2(a.im.doubleValue(), a.re.doubleValue());
      angleA = angleA * m;
      double angleB = Math.atan2(b.im.doubleValue(), b.re.doubleValue());
      angleB = angleB * n;
      double angle = angleA - angleB;
      for (int i = -200; i <= 200; i++) {
        double from = Math.PI * i * 2;
        if (Math.abs(angle - from) < 1e-1)
          return i;
      }
      throw new RuntimeException("Should not happen");
    }
}
