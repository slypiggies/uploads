/*
  Each contestant has a 6-digit code ABCDEF, where ABC is the school code, D is the section (1-5), E is the index of students from the same school and same section (1-3), F is the check digit.
  Assign school codes 001, 002, ... by alphabetical order.
  Note that PCMS will join but has not yet been included in the Excel file (so PCMS needs a school code too). Chu sir should send the data of PCMS students next week or so.
  Individual contestants are usually numbered 901, 902, ... Note that PCA has not yet sent the data of individual contestants.
  My suggestion for check digit: 3A+B+3C+D+3E+F=year of contest (mod 10).
  
  individual: 90?D1F
*/
function genCids() {
  updateDataBuffer();
  var numRows = getNumRows();
  for (var i = 1; i < numRows; i++) {
    if (getData(i, 10) != "individual" && getData(i, 10) != "normal") { // school
    } else { // contestant
      clearData(i, 4);
    }
  }
  
  var schoolId;
  var cntIndividual = 1;
  var cntNormal;
  for (var i = 1; i < numRows; i++) {
    if (getData(i, 10) != "individual" && getData(i, 10) != "normal") { // school
      var t = getData(i, 1);
      for (var j = 0; j < schools.length; j++) {
        if (schools[j] === t) {
          schoolId = j + 1; // 1-based
        }
      }
    } else { // contestant
      var cid = "";
      // ABC
      if (getData(i, 10) === "individual") {
        cid += "9" + pad0(cntIndividual++, 2);
      } else { // normal
        cid += pad0(schoolId, 3);
      }
      // D
      cid += intToString(getData(i, 5) + 1);
      // E
      if (getData(i, 0) === getData(i - 1, 0)) {
        cntNormal++;
      } else {
        cntNormal = 1;
      }
      cid += intToString(cntNormal);
      // F
      var checkDigit;
      for (var j = 0; j < 10; j++) {
        var aa = cid.charAt(0),
            bb = cid.charAt(1),
            cc = cid.charAt(2),
            dd = cid.charAt(3),
            ee = cid.charAt(4);
        var A = stringToInt(aa),
            B = stringToInt(bb),
            C = stringToInt(cc),
            D = stringToInt(dd),
            E = stringToInt(ee);
        if ((3 * A + B + 3 * C + D + 3 * E + j) % 10 === 2020 % 10) {
          checkDigit = j;
        }
      }
      cid += intToString(checkDigit);
      
      setData(i, 4, "\'" + cid);
    }
  }
}
