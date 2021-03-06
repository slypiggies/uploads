function mailMerge(stepSize) {
  updateDataBuffer();
  var out = DocumentApp.openById(outIds[getData(0, 1)]).getBody();
  var x = getData(0, 0);
  var y = x + stepSize;
  out.appendPageBreak();
  out.clear();
  out.setMarginBottom(temps[0].getMarginBottom());
  out.setMarginLeft(temps[0].getMarginLeft());
  out.setMarginRight(temps[0].getMarginRight());
  out.setMarginTop(temps[0].getMarginTop());
  out.appendPageBreak();
  
  var elements = [];
  for (var i = 0; i < temps.length; i++) {
    elements.push([]);
    for (var j = 0; j < temps[i].getNumChildren(); j++) {
      elements[i].push(temps[i].getChild(j).copy());
    }
  }
  
  var numRows = getNumRows();
  var cid;
  var name_zh;
  var name_en;
  var group;
  var school_name_zh;
  var school_id;
  var section_time;
  var seat;
  for (var i = x; i < numRows; i++) {
    Logger.log("i = " + i);
    if (getData(i, 10) != "individual" && getData(i, 10) != "normal") { // school
      if (i > y) {
        y = i;
        break;
      }
      school_name_zh = getData(i, 0);
      var t = getData(i, 1);
      for (var j = 0; j < schools.length; j++) {
        if (schools[j] === t) {
          school_id = j + 1; // 1-based
        }
      }
    } else { // contestant
      cid = getData(i, 4);
      name_zh = getData(i, 1);
      name_en = getData(i, 2);
      group = getData(i, 0);
      school_id = pad0(school_id, 3);
      var groupId = getData(i, 5);
      section_time = sectionTimes[groupId];
      seat = getData(i, 6);
      for (var j = 0; j < elements[groupId].length; j++) {
        if (elements[groupId][j].getType() === DocumentApp.ElementType.LIST_ITEM) {
//          Logger.log(j + " = LIST_ITEM");
          out.appendListItem(elements[groupId][j].copy());
        } else if (elements[groupId][j].getType() === DocumentApp.ElementType.PARAGRAPH) {
//          Logger.log(j + " = PARAGRAPH");
          out.appendParagraph(elements[groupId][j].copy());
        } else if (elements[groupId][j].getType() === DocumentApp.ElementType.TABLE) {
//          Logger.log(j + " = TABLE");
          var tableCopy = elements[groupId][j].copy();
          tableCopy.replaceText("<cid>", cid);
          tableCopy.replaceText("<name_zh>", name_zh);
          tableCopy.replaceText("<name_en>", name_en);
          tableCopy.replaceText("<group>", group);
          tableCopy.replaceText("<school_name_zh>", school_name_zh);
          tableCopy.replaceText("<school_id>", school_id);
          tableCopy.replaceText("<section_time>", section_time);
          tableCopy.replaceText("<seat>", seat);
          out.appendTable(tableCopy);
        }
      }
      out.appendPageBreak();
    }
  }
  
  setData(0, 0, y);
  setData(0, 1, getData(0, 1) + 1);
}
