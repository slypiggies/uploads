function getSchools() {
  updateDataBuffer();
  
  var numRows = getNumRows();
  var vis = {};
  for (var i = 1; i < numRows; i++) {
    if (getData(i, 10) != "individual" && getData(i, 10) != "normal") { // school
      if (vis[getData(i, 1)] != true) {
        vis[getData(i, 1)] = true;
        schools.push(getData(i, 1));
      }
    }
  }
  schools.sort();
}
