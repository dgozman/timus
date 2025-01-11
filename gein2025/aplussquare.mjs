function solve2(n, cnt) {
  if (!n)
    return [];
  if (!cnt)
    return undefined;
  const a = new Array(cnt + 1).fill(0).map(_ => new Array(n + 1).fill(-1));
  a[0][0] = 0;
  for (let i = 0; i < cnt; i++) {
    for (let j = 0; j <= n; j++) {
      if (a[i][j] == -1)
        continue;
      for (let k = 1; j + k * k <= n; k++)
        a[i + 1][j + k * k] = k;
    }
    if (a[i + 1][n] != -1) {
      const result = [];
      let s = n;
      for (let j = i + 1; j > 0; j--) {
        result.push(a[j][s]);
        s -= a[j][s] * a[j][s];
      }
      return result;
    }
  }
  return undefined;
}

function solve(n, cnt) {
  // console.log('solve', n, cnt);
  if (n == 0n)
    return [];
  if (!cnt)
    return undefined;
  if (n > 1000000n) {
    const first = Math.floor(Math.sqrt(Number(n)));
    for (let f = first - 10; f <= first; f++) {
      if (f >= 1) {
        const res = solve(n - BigInt(f) * BigInt(f), cnt - 1);
        if (res) {
          res.push(f);
          return res;
        }
      }
    }
    return undefined;
  }
  return solve2(Number(n), cnt);
}


async function solveOne() {
  const input = await fetch('https://2025.andgein.ru/api/tasks/lagrange', {
    headers: { 'Token': 'purple-wombat-cucumber-3107' },
  });
  const text = await input.text();
  const json = JSON.parse(text);
  const level = json.current_level;
  console.log(`INPUT #${level}: ` + text);
  const index1 = text.indexOf('"N":');
  if (index1 == -1) {
    console.log("CANNOT PARSE 1");
    return;
  }
  const index2 = text.indexOf(',', index1 + 1);
  if (index2 == -1) {
    console.log("CANNOT PARSE 1");
    return;
  }
  const n = BigInt(text.substring(index1 + 4, index2));
  console.log(`N=${n}`);
  const cnt = +json.parameters.max_numbers_count;

  const arr = solve(n, cnt);
  const answer = arr ? arr.join(' ') : 'NO';
  if (answer == 'NO') {
    console.log('no answer???');
    return false;
  }
  const result = { level, answer };
  console.log(`ANSWER #${level}: ` + answer);

  const output = await fetch('https://2025.andgein.ru/api/tasks/lagrange', {
    method: 'POST',
    headers: { 'Token': 'purple-wombat-cucumber-3107', 'Content-Type': 'application/json' },
    body: JSON.stringify(result),
  });
  const res = await output.json();
  console.log(`OUTPUT #${level}: ` + JSON.stringify(res));
  return res.is_correct;
}

for (let test = 15; test <= 20; test++) {
  if (!await solveOne())
    break;
}
