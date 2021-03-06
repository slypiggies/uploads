/*
  The Heats will be held in 5 sections.
  There are 3 venues (A = Hall, B = Exhibition Centre, C = Gym).
  Subject to maximum capacity (check with Chu sir), assign contestants to the three venues.
  Students from the same school should go to the same venue (throughout the 5 sections).
  Within each venue, assign seats randomly so that students from the same school will not sit together in general.
*/
function genSeats() {
  updateDataBuffer();
  var numRows = getNumRows();
  for (var i = 1; i < numRows; i++) {
    if (getData(i, 10) != "individual" && getData(i, 10) != "normal") { // school
    } else { // contestant
      clearData(i, 6);
    }
  }
  
  // assign schools to venues
  var capacitiesCopy = [];
  for (var i = 0; i < capacities.length; i++) {
    capacitiesCopy.push(capacities[i]);
  }
//  Logger.log(schools.length);
  var schoolToVenues = [];
  for (var i = 0; i < schools.length; i++) {
    var j = 0;
    while (j < capacitiesCopy.length && capacitiesCopy[j] < avgNumCsPerSchool) { // not enough
      j++;
    }
    if (j === capacitiesCopy.length) {
      Logger.log("********* OOPS: not enough venues *********");
      Logger.log("schools[i] = " + schools[i]);
      return;
    }
    capacitiesCopy[j] -= avgNumCsPerSchool;
    schoolToVenues.push(String.fromCharCode(j + "A".charCodeAt(0)));
//    Logger.log(schools[i] + " " + schoolToVenues[i]);
//    Logger.log("remaining: " + capacitiesCopy[0] + " " + capacitiesCopy[1] + " " + capacitiesCopy[2]);
  }
  
  for (var i = 0; i < sectionTimes.length; i++) {
    for (var j = 0; j < 3; j++) {
      var forEachSection = i;
      var forEachVenue = String.fromCharCode(j + "A".charCodeAt(0));
      var whatSection;
      var whatVenue;
      
      var poolOfCsRow = []; // pool of contestants, contains the row position from data, -1 means seated
      var poolOfCsSchoolId = []; // pool of contestants, contains their school ids
      // select the pool from all contestants, with matched section and venue
      for (var k = 1; k < numRows; k++) {
        if (getData(k, 10) != "individual" && getData(k, 10) != "normal") { // school
          var t = getData(k, 1);
          var schoolId;
          for (var l = 0; l < schools.length; l++) {
            if (t === schools[l]) {
              schoolId = l;
            }
          }
          whatVenue = schoolToVenues[schoolId];
        } else { // contestant
          whatSection = getData(k, 5);
          if (forEachSection === whatSection && forEachVenue === whatVenue) { // match
            poolOfCsRow.push(k);
            poolOfCsSchoolId.push(schoolId);
          }
        }
      }
      
//      Logger.log("---");
//      Logger.log("section = " + forEachSection);
//      Logger.log("venue = " + forEachVenue);
//      for (var k = 0; k < poolOfCsRow.length; k++) {
//        Logger.log(poolOfCsRow[k]);
//      }
      
      var p = []; // permutation
      var poolOfCsRowCopy = [];
      var poolOfCsSchoolIdCopy = [];
      for (var k = 0; k < poolOfCsRow.length; k++) {
        p.push(k);
        poolOfCsRowCopy.push(poolOfCsRow[k]);
        poolOfCsSchoolIdCopy.push(poolOfCsSchoolId[k]);
      }
      p = randShuffle(p);
      for (var k = 0; k < p.length; k++) {
        poolOfCsRow[k] = poolOfCsRowCopy[p[k]];
        poolOfCsSchoolId[k] = poolOfCsSchoolIdCopy[p[k]];
      }
      
//      Logger.log("---");
//      for (var k = 0; k < poolOfCsRow.length; k++) {
//        Logger.log(poolOfCsRow[k]);
//      }
//      return;
      
      var venueNumRows = myVenueNumsRows[j], venueNumCols = myVenueNumsCols[j];
      var actualSeatPlan = []; // contains school ids for neighbour-checking, -1 means empty
      var ii, jj;
      for (ii = 0; ii < venueNumRows; ii++) {
        actualSeatPlan.push([]);
        for (jj = 0; jj < venueNumCols; jj++) {
          actualSeatPlan[ii].push(-1);
        }
      }
      
//      Logger.log("---");
//      Logger.log(actualSeatPlan.length);
//      Logger.log(actualSeatPlan[0].length);
//      return;
      
      ii = 0;
      jj = 0;
      var dx = [-1, -1, -1, 0, 0, 1, 1, 1], dy = [-1, 0, 1, -1, 1, -1, 0, 1];
      // for each seat
      for (var k = 1; k <= capacities[j]; k++) { // seats are 1-based
        var kk;
        for (kk = 0; kk < poolOfCsRow.length; kk++) {
          if (poolOfCsRow[kk] != -1) {
            var validPos = true;
            for (var l = 0; l < 8; l++) { // for each direction
              if (ii + dx[l] < 0 || ii + dx[l] >= venueNumRows ||
                  jj + dy[l] < 0 || jj + dy[l] >= venueNumCols) { // out of bound neighbour
                continue;
              }
              if (actualSeatPlan[ii + dx[l]][jj + dy[l]] === -1) { // neighbour is empty
                continue;
              }
              if (poolOfCsSchoolId[kk] === actualSeatPlan[ii + dx[l]][jj + dy[l]]) { // not ok
                validPos = false;
              }
            }
            if (validPos) {
              break;
            }
          }
        }
        
//        Logger.log(kk);
//        Logger.log(k + " " + (jj * venueNumRows + ii + 1));
        
        if (kk === poolOfCsRow.length) { // all contestants from this section and venue are seated
          break; // break the "for each seat in this venue" loop
        }
        setData(poolOfCsRow[kk], 6, forEachVenue + pad0(jj * venueNumsRows[j] + ii + 1, 3)); // when assigning real seat's position, use default venueNums[]
        actualSeatPlan[ii][jj] = poolOfCsSchoolId[kk];
        poolOfCsRow[kk] = -1; // set to seated
        
        ii++;
        if (ii >= venueNumRows) {
          ii = 0;
          jj++;
        }
      }
      
      // check if all seated
      for (var k = 0; k < poolOfCsRow.length; k++) {
        if (poolOfCsRow[k] != -1) {
          Logger.log("********* OOPS: not all seated *********");
          Logger.log("section = " + forEachSection);
          Logger.log("venue = " + forEachVenue);
          Logger.log("poolOfCsRow[k] = " + poolOfCsRow[k]);
        }
      }
      
//      Logger.log(ii);
//      Logger.log(jj);
//      return;
    }
  }
}
