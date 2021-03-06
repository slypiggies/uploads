function dataToData3() {
  data3.clear();
  var row = 0, col = 0;
  setData3(row, col++, "group_id");
  setData3(row, col++, "venue");
  setData3(row, col++, "seat");
  for (var i = 0; i < 6; i++) setData3(row, col++, "cid_digit" + intToString(i));
  for (var i = 0; i < 10; i++) setData3(row, col++, "cid_row" + intToString(i));
  setData3(row, col++, "name_zh");
  setData3(row, col++, "name_en");
  setData3(row, col++, "cid");
  setData3(row, col++, "school_name_zh");
  setData3(row, col++, "school_name_en");
  row++; col = 0;
  
  updateDataBuffer();
  var numRows = getNumRows();
  
  var group_id;
  var venue;
  var seat;
  var cid_digit = [];
  for (var i = 0; i < 6; i++) cid_digit.push(0);
  var cid_row = [];
  for (var i = 0; i < 10; i++) cid_row.push("");
  var name_zh;
  var name_en;
  var cid;
  var school_name_zh;
  var school_name_en;
  for (var i = 1; i < numRows; i++) {
    Logger.log("i = " + i);
    if (getData(i, 10) != "individual" && getData(i, 10) != "normal") { // school
      school_name_zh = getData(i, 0);
      school_name_en = getData(i, 1);
      var t = getData(i, 1);
      for (var j = 0; j < schools.length; j++) {
        if (schools[j] === t) {
          school_id = j + 1; // 1-based
        }
      }
    } else { // contestant
      group_id = getData(i, 5);
      venue = getData(i, 6).charAt(0);
      seat = getData(i, 6).substr(1, 3);
      cid = getData(i, 4);
      for (var j = 0; j < 6; j++) cid_digit[j] = String.fromCharCode(cid.charCodeAt(j) - "0".charCodeAt(0) + "i".charCodeAt(0));
      for (var j = 0; j < 10; j++) {
        cid_row[j] = "";
        for (var k = 0; k < 6; k++) {
          if (cid.charCodeAt(k) - "0".charCodeAt(0) === j) {
            cid_row[j] += "{";
          } else {
            cid_row[j] += "}";
          }
          cid_row[j] += "	";
        }
      }
      name_zh = getData(i, 1);
      name_en = getData(i, 2);
      
      setData3(row, col++, group_id + 1);
      setData3(row, col++, venue);
      setData3(row, col++, "\'" + seat);
      for (var j = 0; j < 6; j++) setData3(row, col++, cid_digit[j]);
      for (var j = 0; j < 10; j++) setData3(row, col++, cid_row[j]);
      setData3(row, col++, name_zh);
      setData3(row, col++, name_en);
      setData3(row, col++, "\'" + cid);
      setData3(row, col++, school_name_zh);
      setData3(row, col++, school_name_en);
      row++; col = 0;
    }
  }
}
