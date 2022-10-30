import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.math.BigInteger;
import java.util.Arrays;
import java.util.Random;

public class timus1677 {
    static BufferedReader in;
    static PrintWriter out;

    public static void main(String[] args) throws Exception {
        in = new BufferedReader(new InputStreamReader(System.in));
        out = new PrintWriter(System.out);
        solve();
        out.close();
    }

    public static void solve() throws Exception {
        int k = Integer.parseInt(in.readLine().trim());
        char[] s = in.readLine().trim().toCharArray();
        int n = s.length;
//        int k = 26;
//        int n = 30000;
//        char[] s = new char[n];
//        Random random = new Random(42);
//        for (int i = 0; i < n; i++)
//            s[i] = (char) ('a' + random.nextInt(26));

        int[] pi = new int[n + 1];
        pi[1] = 0;

        int[][] pj = new int[n][k];
        for (int j = 0; j < k; j++)
            pj[0][j] = 0;
        pj[0][s[0] - 'a'] = 1;

        for (int i = 2; i <= n; i++) {
            int j = pi[i - 1];
            while (j > 0 && s[j] != s[i - 1])
                j = pi[j];
            if (s[j] == s[i - 1])
                j++;
            pi[i] = j;

            j = pi[i - 1];
            for (int ch = 0; ch < k; ch++)
                pj[i - 1][ch] = pj[j][ch];
            pj[i - 1][s[i - 1] - 'a'] = i;
        }


        BigInteger[] r = new BigInteger[n + 1];
        for (int i = 0; i <= n; i++)
            r[i] = BigInteger.ZERO;
        BigInteger c = BigInteger.ONE;
        BigInteger bk = BigInteger.valueOf(k);

        int[] js = new int[k];
        for (int i = n - 1; i >= 0; i--) {
            //k * pi = k + p(i+1) + pa + pb + ... + pz
            //k * c * pi = k * c + c*p(i+1) + c * pa + c * pb + .... + c * pz
            int jc = 0;
            for (int ch = 0; ch < k; ch++) {
                if (ch + 'a' == s[i])
                    continue;
                js[jc++] = pj[i][ch];
            }
            Arrays.sort(js, 0, jc);
            for (int j = 0; j < jc; ) {
                int v = js[j];
                int j2 = j + 1;
                while (j2 < jc && js[j2] == v)
                    j2++;
                int count = j2 - j;
                if (count == 1) {
                    r[v] = r[v].add(c);
                } else {
                    BigInteger tmp = c.multiply(BigInteger.valueOf(count));
                    r[v] = r[v].add(tmp);
                }
                j = j2;
            }
            c = c.multiply(bk);
            r[n] = r[n].add(c);
            c = c.subtract(r[i]);
            r[i] = null;
        }
        out.println(r[n]);
//        out.println(c);
//        BigInteger result = r[n].divide(c);
//        out.println(result);
    }
}


//4
//abcabacabc
//i=4
//abc