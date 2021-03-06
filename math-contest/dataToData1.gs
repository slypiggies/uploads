/*
  this exists because Mark = wtf and need to use Microsoft Word to do the mail merge
  this converts data (which is perfectly fine) to data1
  data1 then should be downloaded as Microsoft Excel file, to do the rest
*/
function dataToData1() {
  data1.clear();
  setData1(0, 0, "shorten_group");
  setData1(0, 1, "cid");
  setData1(0, 2, "name_zh");
  setData1(0, 3, "name_en");
  setData1(0, 4, "group");
  setData1(0, 5, "school_name_zh");
  setData1(0, 6, "school_id");
  setData1(0, 7, "section_time");
  setData1(0, 8, "seat");
  
  updateDataBuffer();
  var numRows = getNumRows();
  var row = 1, col = 0;
  
  var shorten_group;
  var cid;
  var name_zh;
  var name_en;
  var group;
  var school_name_zh;
  var school_id;
  var section_time;
  var seat;
  for (var i = 1; i < numRows; i++) {
    Logger.log("i = " + i);
    if (getData(i, 10) != "individual" && getData(i, 10) != "normal") { // school
      school_name_zh = getData(i, 0);
      var t = getData(i, 1);
      for (var j = 0; j < schools.length; j++) {
        if (schools[j] === t) {
          school_id = j + 1; // 1-based
        }
      }
    } else { // contestant
      shorten_group = getData(i, 7);
      cid = getData(i, 4);
      name_zh = getData(i, 1);
      name_en = getData(i, 2);
      group = getData(i, 0);
      school_id = pad0(school_id, 3);
      var groupId = getData(i, 5);
      section_time = sectionTimes[groupId];
      seat = getData(i, 6);
      
      setData1(row, col++, shorten_group);
      setData1(row, col++, "\'" + cid);
      setData1(row, col++, name_zh);
      setData1(row, col++, name_en);
      setData1(row, col++, group);
      setData1(row, col++, school_name_zh);
      setData1(row, col++, "\'" + school_id);
      setData1(row, col++, section_time);
      setData1(row, col++, seat);
      row++; col = 0;
    }
  }
}
