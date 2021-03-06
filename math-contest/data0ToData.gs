function data0ToData() {
  data.clear();
  
  // individual
  var numRows = data01.getDataRange().getValues().length;
  var row = 1, col = 0;
  for (var i = 2; i < numRows; i++) {
    setData(row, col++, getData01(i, 1)); // school_name_zh
    setData(row, col++, getData01(i, 2)); // school_name_en
    setData(row, col++, getData01(i, 4)); // school_address_zh
    row++; col = 0;
    
    var t = getData01(i, 5),
        t1 = getData01(i, 5);
    if (t != "中一" && t != "中二" && t != "中三" && t != "中四") {
      t = "高中 (中五至中六)";
      t1 = "高中";
    }
    setData(row, col++, t); // group
    setData(row, 7, t1); // shorten group
    var groupId;
    if (t === "中一") {
      groupId = 0;
    } else if (t === "中二") {
      groupId = 1;
    } else if (t === "中三") {
      groupId = 2;
    } else if (t === "中四") {
      groupId = 3;
    } else if (t === "高中 (中五至中六)") {
      groupId = 4;
    }
    setData(row, 5, groupId);
    setData(row, col++, getData01(i, 12)); // name_zh
    t = getData01(i, 13) + " " + getData01(i, 14);
    t = t.toUpperCase();
    setData(row, col++, t); // name_en
    setData(row, 10, "individual");
    row++; col = 0;
  }
  
  // normal
  numRows = data00.getDataRange().getValues().length;
  for (var i = 2; i < numRows; i++) {
    for (var j = 0; j < 7; j++) {
      setData(row, col++, getData00(i, j + 1)); // school_name_zh, school_name_en, school_address_zh, ...
    }
    setData(row, col++, getData00(i, 8));
    setData(row, col++, getData00(i, 9));
    setData(row, col++, getData00(i, 25));
    setData(row, col++, getData00(i, 41));
    setData(row, col++, getData00(i, 57));
    setData(row, col++, getData00(i, 73));
    row++; col = 0;
    
    var cols = [11, 16, 21, 
                27, 32, 37, 
                43, 48, 53, 
                59, 64, 69, 
                75, 80, 85];
    for (var j = 0; j < cols.length; j++) {
      if (getData00(i, cols[j]) != "" || getData00(i, cols[j] + 1) != "" || getData00(i, cols[j] + 2) != "") {
        var t = getData00(i, cols[j] - 1),
            t1 = getData00(i, cols[j] - 1);
        if (t != "中一" && t != "中二" && t != "中三" && t != "中四") {
          t = "高中 (中五至中六)";
          t1 = "高中";
        }
        setData(row, col++, t); // group
        setData(row, 7, t1); // shorten group
        var groupId;
        if (t === "中一") {
          groupId = 0;
        } else if (t === "中二") {
          groupId = 1;
        } else if (t === "中三") {
          groupId = 2;
        } else if (t === "中四") {
          groupId = 3;
        } else if (t === "高中 (中五至中六)") {
          groupId = 4;
        }
        setData(row, 5, groupId);
        setData(row, col++, getData00(i, cols[j])); // name_zh
        t = getData00(i, cols[j] + 1) + " " + getData00(i, cols[j] + 2);
        t = t.toUpperCase();
        setData(row, col++, t); // name_en
        setData(row, col++, getData00(i, cols[j] + 3)); // gender
        setData(row, 10, "normal");
        row++; col = 0;
      }
    }
  }
}
