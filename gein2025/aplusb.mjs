function toBigInt(a) {
  if (a.endsWith('₂'))
    a = '0b' + a.slice(0, -1);
  const last = a.charCodeAt(a.length - 1);
  if (last == 178) {
    a = a.slice(0, -1);
    return BigInt(a) ** BigInt(2);
  }
  if (last == 179) {
    a = a.slice(0, -1);
    return BigInt(a) ** BigInt(3);
  }
  if (last >= 8305 && last <= 8314) {
    const power = last - 8304;
    a = a.slice(0, -1);
    return BigInt(a) ** BigInt(power);
  }
  return BigInt(a);
}

function fromConst(a, precision) {
  const x = Math.floor(a);
  const y = (a - x) * Math.pow(10, precision + 2);
  return [BigInt(x), precision + 2, BigInt(Math.floor(y))];
}

function toPair(a, precision) {
  if (a == '𝜋')
    return fromConst(Math.PI, precision);
  if (a == '𝜋²')
    return fromConst(Math.PI * Math.PI, precision);
  if (a == '𝜋³')
    return fromConst(Math.PI * Math.PI * Math.PI, precision);
  if (a == '𝜋⁴')
    return fromConst(Math.PI * Math.PI * Math.PI * Math.PI, precision);
  if (a == '𝜋⁵')
    return fromConst(Math.PI * Math.PI * Math.PI * Math.PI * Math.PI, precision);
  if (a == 'ℯ')
    return fromConst(Math.E, precision);
  if (a == 'ℯ²')
    return fromConst(Math.E * Math.E, precision);
  if (a == 'ℯ³')
    return fromConst(Math.E * Math.E * Math.E, precision);
  if (a == 'ℯ⁴')
    return fromConst(Math.E * Math.E * Math.E * Math.E, precision);
  if (a == 'ℯ⁵')
    return fromConst(Math.E * Math.E * Math.E * Math.E * Math.E, precision);
  if (a.includes('.')) {
    const [x, y] = a.split('.');
    return [toBigInt(x), y.length, toBigInt(y)];
  }
  return [toBigInt(a), 0, 0n];
}

function add(a, b, precision) {
  const ra = toPair(a, precision);
  const rb = toPair(b, precision);
  console.log('ra', ra, 'rb', rb);
  while (ra[1] < rb[1]) {
    ra[2] *= 10n;
    ra[1]++;
  }
  while (rb[1] < ra[1]) {
    rb[2] *= 10n;
    rb[1]++;
  }
  console.log('ra', ra, 'rb', rb);
  let before = ra[0] + rb[0];
  let after = ra[2] + rb[2];
  console.log('before', before, 'after', after);
  const p = 10n ** BigInt(ra[1]);
  if (after >= p) {
    const mod = after % p;
    before += (after - p) / mod;
    after = mod;
  }
  console.log('before', before, 'after', after);
  return String(before) + '.' + String(after).padStart('0', ra[1]);
  // return (toBigInt(a) + toBigInt(b)).toString();
}

async function solveOne() {
  const input = await fetch('https://2025.andgein.ru/api/tasks/a-plus-b', {
    headers: { 'Token': 'purple-wombat-cucumber-3107' },
  });
  const json = await input.json();
  const level = json.current_level;
  console.log(`INPUT #${level}: ` + JSON.stringify(json));
  const a = json.parameters.A;
  const b = json.parameters.B;
  const precision = json.parameters.precision;

  const answer = String(add(a, b, precision));
  const result = { level, answer };
  console.log(`ANSWER #${level}: ` + answer);

  const output = await fetch('https://2025.andgein.ru/api/tasks/a-plus-b', {
    method: 'POST',
    headers: { 'Token': 'purple-wombat-cucumber-3107', 'Content-Type': 'application/json' },
    body: JSON.stringify(result),
  });
  const res = await output.json();
  console.log(`OUTPUT #${level}: ` + JSON.stringify(res));
  return res.is_correct;
}

for (let test = 83; test <= 200; test++) {
  if (!await solveOne())
    break;
}
