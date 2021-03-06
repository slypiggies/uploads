function dataToData2() {
  data2.clear();
  setData2(0, 0, "venue");
  setData2(0, 1, "seats_range");
  setData2(0, 2, "shorten_group");
  var row = 0, col = 3;
  for (var i = 0; i < 10; i++) setData2(row, col++, "seat" + intToString(i + 1));
  for (var i = 0; i < 10; i++) setData2(row, col++, "cid" + intToString(i + 1));
  for (var i = 0; i < 10; i++) setData2(row, col++, "name_zh" + intToString(i + 1));
  for (var i = 0; i < 10; i++) setData2(row, col++, "name_en" + intToString(i + 1));
  setData2(row, col++, "page_X");
  setData2(row, col++, "of_Y");
  row++; col = 0;
  
  updateDataBuffer();
  var numRows = getNumRows();
  
  var contestants = []; // e.g. i = 31012: contestants[i] = contestant in 中四組, sitting at B012
  var contestantExists = [];
  for (var i = 0; i < 50000; i++) contestants.push(new Object());
  for (var i = 0; i < 50000; i++) contestantExists.push(false);
  
  // fill contestants
  for (var i = 1; i < numRows; i++) {
    Logger.log("i = " + i);
    if (getData(i, 10) != "individual" && getData(i, 10) != "normal") { // school
    } else { // contestant
      var contestant = new Object();
      contestant.seat = getData(i, 6);
      contestant.cid = getData(i, 4);
      contestant.name_zh = getData(i, 1);
      contestant.name_en = getData(i, 2);
      var t = getData(i, 5) * 10000 + hashSeat(contestant.seat);
      contestants[t] = contestant;
      contestantExists[t] = true;
    }
  }
  
  var xCnts = []; // in hex, for page numbers
  for (var i = 0; i < 16 * 16; i++) xCnts.push(0);
  var hexRecords = [];
  for (var k = 0; k < 5; k++) {
    for (var i = 1; i < 3000; i += 10) {
      var noCs = true; // no contestants
      for (var j = i; j < i + 10; j++) {
        if (contestantExists[k * 10000 + j] === true) {
          noCs = false;
        }
      }
      if (!noCs) {
        var venueId = i / 1000;
        var venue = String.fromCharCode(venueId + "A".charCodeAt(0));
        var seats_range = venue + pad0(i % 1000, 3) + " to " + venue + pad0(i % 1000 + 10 - 1, 3);
        Logger.log(seats_range);
        var shorten_group;
        if (k === 0) {
          shorten_group = "中一";
        } else if (k === 1) {
          shorten_group = "中二";
        } else if (k === 2) {
          shorten_group = "中三";
        } else if (k === 3) {
          shorten_group = "中四";
        } else if (k === 4) {
          shorten_group = "高中";
        }
        shorten_group += "組";
        
        setData2(row, col++, venue);
        setData2(row, col++, seats_range);
        setData2(row, col++, shorten_group);
        for (var j = i; j < i + 10; j++) setData2(row, col++, contestantExists[k * 10000 + j] ? contestants[k * 10000 + j].seat : "");
        for (var j = i; j < i + 10; j++) setData2(row, col++, contestantExists[k * 10000 + j] ? "\'" + contestants[k * 10000 + j].cid : "");
        for (var j = i; j < i + 10; j++) setData2(row, col++, contestantExists[k * 10000 + j] ? contestants[k * 10000 + j].name_zh : "");
        for (var j = i; j < i + 10; j++) setData2(row, col++, contestantExists[k * 10000 + j] ? contestants[k * 10000 + j].name_en : "");
        var t = (k + 1) * 16 + (venue.charCodeAt(0) - "A".charCodeAt(0) + 10);
        hexRecords.push(t);
        xCnts[t]++;
        setData2(row, col++, "Page " + xCnts[t]);
        row++; col = 0;
      }
    }
  }
  
  for (var i = 0; i < hexRecords.length; i++) {
    setData2(i + 1, 44, "of " + xCnts[hexRecords[i]]);
  }
}
