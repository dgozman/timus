import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.math.BigInteger;

public class timus1013 {
    static BufferedReader in;
    static PrintWriter out;

    public static void main(String[] args) throws Exception {
        in = new BufferedReader(new InputStreamReader(System.in));
        out = new PrintWriter(System.out);
        solve();
        out.close();
    }

    public static BigInteger[][] mul(BigInteger[][] a, BigInteger[][] b, BigInteger mod) {
        BigInteger[][] r = new BigInteger[2][2];
        r[0][0] = a[0][0].multiply(b[0][0]).add(a[0][1].multiply(b[1][0])).mod(mod);
        r[0][1] = a[0][0].multiply(b[0][1]).add(a[0][1].multiply(b[1][1])).mod(mod);
        r[1][0] = a[1][0].multiply(b[0][0]).add(a[1][1].multiply(b[1][0])).mod(mod);
        r[1][1] = a[1][0].multiply(b[0][1]).add(a[1][1].multiply(b[1][1])).mod(mod);
        return r;
    }

    public static void solve() throws Exception {
        long n = Long.parseLong(in.readLine().trim()) - 1;
        BigInteger k = new BigInteger(in.readLine().trim());
        BigInteger mod = new BigInteger(in.readLine().trim());

        BigInteger k1 = k.subtract(BigInteger.ONE);
        BigInteger[][] w = new BigInteger[][] { {BigInteger.ZERO, BigInteger.ONE}, {k1, k1} };
        BigInteger[][] r = new BigInteger[][] { { BigInteger.ONE, BigInteger.ZERO}, {BigInteger.ZERO, BigInteger.ONE} };
        while (n > 0) {
            if (n % 2 > 0) {
                r = mul(r, w, mod);
            }
            w = mul(w, w, mod);
            n /= 2;
        }

        BigInteger res = r[1][0].add(r[1][1].multiply(k1)).mod(mod);
        out.println(res);
    }
}
