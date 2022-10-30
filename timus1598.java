import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.math.BigInteger;
import java.util.HashMap;
import java.util.Random;
import java.util.StringTokenizer;

public class timus1598 {
    static BufferedReader in;
    static PrintWriter out;

    public static void main(String[] args) throws Exception {
        in = new BufferedReader(new InputStreamReader(System.in));
        out = new PrintWriter(System.out);
        solve();
        out.close();
    }

    public static void solve() throws Exception {
        // https://cr.yp.to/dlog/cuberoot-20120919.pdf

        StringTokenizer st = new StringTokenizer(in.readLine());
        int N = Integer.parseInt(st.nextToken());
        int L = Integer.parseInt(st.nextToken());
        BigInteger q = new BigInteger(st.nextToken());
        BigInteger p = new BigInteger(st.nextToken());
        BigInteger g = new BigInteger(st.nextToken());
        BigInteger y = new BigInteger(st.nextToken());
        BigInteger H = new BigInteger(st.nextToken());

        long W = Math.round(Math.sqrt(q.longValue()));
        HashMap<Long, Long> map = new HashMap<>();
        BigInteger hgj = y;
        for (long j = 0; j < W; j++) {
            map.put(hgj.longValue(), j);
            hgj = hgj.multiply(g).mod(p);
        }
        BigInteger gw = g.modPow(BigInteger.valueOf(W), p);
        BigInteger gwi = gw;
        long i = 1;
        while (!map.containsKey(gwi.longValue())) {
            i++;
            gwi = gwi.multiply(gw).mod(p);
        }
        long xx = i * W - map.get(gwi.longValue());
        if (xx <= 0 || xx >= q.longValue())
            throw new AssertionError("Oh my");
        BigInteger x = BigInteger.valueOf(xx);
//        out.println(x);

        Random random = new Random(42);
        while (true) {
            long kk = Math.abs(random.nextLong()) % (q.longValue() - 1) + 1;
//            long kk = 2;
            BigInteger k = BigInteger.valueOf(kk);
            BigInteger r = g.modPow(k, p).mod(q);
            if (r.equals(BigInteger.ZERO))
                continue;
            BigInteger s = k.modInverse(q).multiply(H.add(r.multiply(x))).mod(q);
            if (s.equals(BigInteger.ZERO))
                continue;
            out.println(r.toString() + " " + s.toString());
            break;
        }
    }
}
