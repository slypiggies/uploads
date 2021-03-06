function helloWorld() {
  Logger.log("Hello!");
}

/*
  for each occurrence of string t in string s, replace it by u
  @return the replaced string (s is not modified)
*/
function findAndReplace(s, t, u) {
  var res = "";
  for (var i = 0; i < s.length; i++) {
    var flag = true;
    for (var j = 0; j < t.length; j++) {
      if (i + j >= s.length) {
        flag = false;
        break;
      }
      if (s[i + j] != t[j]) {
        flag = false;
      }
    }
    if (flag) {
      res += u;
      i += t.length - 1;
    } else {
      res += s.charAt(i);
    }
  }
  return res;
}

function intToString(x) {
  return x.toString();
}

function stringToInt(s) {
  var res = 0;
  for (var i = 0; i < s.length; i++) {
    res *= 10;
    res += s.charCodeAt(i) - "0".charCodeAt(0);
  }
  return res;
}

function toA1(s, x) {
  return s + intToString(x);
}

// s is not modified
function stringReverse(s) {
  var res = "";
  for (var i = s.length - 1; i >= 0; i--) {
    res += s.charAt(i);
  }
  return res;
}

/*
  pad int x by zeroes, total length = y
  @return padded string
*/
function pad0(x, y) {
  var res = intToString(x);
  res = stringReverse(res);
  while (res.length < y) {
    res += "0";
  }
  res = stringReverse(res);
  return res;
}

// @return a random number in [0, x)
function rand(x) {
  return Math.floor(Math.random() * x);
}

// @return a randomly shuffled array a, without modifying it
function randShuffle(a) {
  var ids = [];
  for (var i = 0; i < a.length; i++) {
    ids.push(i);
  }
  var res = [];
  for (var i = a.length; i > 0; i--) {
    var ins = rand(i);
    res.push(a[ids[ins]]);
    ids[ins] = ids[i - 1];
  }
  return res;
}

/*
  @return hash value of seat
  e.g. s = "B012", return 1012
*/
function hashSeat(s) {
  var res = (s.charCodeAt(0) - "A".charCodeAt(0)) * 1000;
  res += stringToInt(s.substr(1, 3));
  return res;
}
