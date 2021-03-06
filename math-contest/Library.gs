function getData00(x, y) { // buffered
  return cells00[x][y];
}

function getData01(x, y) { // buffered
  return cells01[x][y];
}

function getData(x, y) { // buffered
  return cells[x][y];
}

function setData(x, y, z) { // live
  data.getRange(x + 1, y + 1).setValue(z);
}

function setData1(x, y, z) { // live
  data1.getRange(x + 1, y + 1).setValue(z);
}

function setData2(x, y, z) { // live
  data2.getRange(x + 1, y + 1).setValue(z);
}

function setData3(x, y, z) { // live
  data3.getRange(x + 1, y + 1).setValue(z);
}

function clearData(x, y) { // same as setData(x, y, "") except also clears formats and rules
  data.getRange(x + 1, y + 1).clear();
}

function updateDataBuffer() {
  cells = data.getDataRange().getValues();
}

function getNumRows() {
  return cells.length;
}

function mailMergeInit() {
  setData(0, 0, 1); // start from data[0][0]-th row
  setData(0, 1, 0); // set output to outIds[data[0][1]]
}
