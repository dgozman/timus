import java.io.*;
import java.math.*;
import java.util.*;

public class timus1948 {
    static BufferedReader in;
    static PrintWriter out;

    public static void main(String[] args) throws Exception {
        in = new BufferedReader(new InputStreamReader(System.in));
        out = new PrintWriter(System.out);
        solve();
        out.close();
    }

    public static boolean check(BigInteger a, BigInteger b, BigInteger c, long kLong) {
      // k^2(-aa) + (aa+3bb-12ac) < 0
      BigInteger k = BigInteger.valueOf(kLong);
      BigInteger aa = a.multiply(a);
      BigInteger bb = b.multiply(b);
      BigInteger ac = a.multiply(c);
      BigInteger kk = k.multiply(k);
      return aa.add(bb.multiply(BigInteger.valueOf(3))).subtract(ac.multiply(BigInteger.valueOf(12))).subtract(kk.multiply(aa)).compareTo(BigInteger.ZERO) < 0;
    }

    public static void solve() throws Exception {
        int tests = Integer.parseInt(in.readLine().trim());
        for (int t = 0; t < tests; t++) {
          StringTokenizer st = new StringTokenizer(in.readLine());
          BigInteger a = new BigInteger(st.nextToken());
          BigInteger b = new BigInteger(st.nextToken());
          BigInteger c = new BigInteger(st.nextToken());

          if (check(a, b, c, 1)) {
            out.println(1);
          } else {
            double ba = b.doubleValue() / a.doubleValue();
            double k2 = -12 * c.doubleValue() / a.doubleValue() + 1 + 3 * ba * ba;
            if (k2 < 0 && k2 > 1e-9)
              k2 = 0;
            long kresult = -1;
            if (k2 >= 0) {
              long kcenter = Math.round(Math.sqrt(k2));
              for (long k = kcenter - 10; k <= kcenter + 10; k++) {
                if (k >= 1 && check(a, b, c, k)) {
                  kresult = k;
                  break;
                }
              }
            }
            out.println(kresult == -1 ? "Infinity" : kresult);
          }
        }
    }
}

/*

axx+bx+c
a(xx+2x+1)+b(x+1)+c
a(xx+4x+4)+b(x+2)+c

a(xx+x*2(k-1)+(k-1)^2)+b(x+k-1)+c


a(xxk + x*2*(0+...k-1) +(0^2+1^2+...(k-1)^2)) + b(xk + (0+..k-1))+c*k

x^2 * ak + x * ak(k-1) + x * bk + a*(k-1)k*(2k-1)/6 + bk(k-1)/2 + ck = 0

A=ak
B=ak(k-1)+bk=akk-ak+bk
C=ak(k-1)(2k-1)/6+bk(k-1)/2+ck=akkk/3-akk/3-akk/6+ak/6+bkk/2-bk/2+ck
D=(akk-ak+bk)(akk-ak+bk)-4ak(akkk/3-akk/3-akk/6+ak/6+bkk/2-bk/2+ck)<0
aak^4-aak^3+abk^3-aak^3+aak^2-abk^2+abk^3-abk^2+bbk^2-4/3aak^4+4/3aak^3+2/3aak^3-2/3aak^2-2abk^3+2abk^2-4ack^2<0

k^4(aa-4/3aa) + k^3(-aa+ab-aa+ab+4/3aa+2/3aa-2ab) + k^2(aa-ab-ab+bb-2/3aa+2ab-4ac)<0

mink so that: k^2(-aa) + (aa+3bb-12ac) < 0

k^2=(-12ac+aa+3bb)/aa=(-12c/a + 1 + 3bb/aa)


*/
