var data = SpreadsheetApp.openById("1gIIToGklSkm7L4ZBmGM7pXLb9mLtwTP6loAplvJcdgM").getSheets()[0], // processed data
    data1 = SpreadsheetApp.openById("137IM97yFfb-b3eivh37BYkGJ8YyQNGDwavW0eQhlTms").getSheets()[0], // 初賽准考證
    data2 = SpreadsheetApp.openById("1m1RddvmMu1DP6HyDtbwQDmJqkHQxQQHb_KHNT8az-O4").getSheets()[0], // Roll Call Sheet
    data3 = SpreadsheetApp.openById("1XS-58E-Tvl04aUY02PGFKrZaxyn6OUqmEeYbELJ4vGc").getSheets()[0]; // Answer Sheet
var cells; // buffered
var temps = [ // templates
  DocumentApp.openById("1CDR3X7o_-wMGVZ504vu5KWIau7NhZW99xj3nYY1sqWk").getBody(), // group0
  DocumentApp.openById("1CzsEtUmVKu1C9Fw7t7l-EfVzC3JrqyT-KdlpzfwvC8U").getBody(), // group1
  DocumentApp.openById("1OuFqCt-jziaqssNUk2s6w0ezvL74zsXxw7r_0a6D4TM").getBody(), // group2
  DocumentApp.openById("1oO08a4-viHVgtaPGwv-2WnGTfTcbrVG-wJJryLzioxs").getBody(), // group3
  DocumentApp.openById("16TXHLW7na9mkW_l2_pDxIA6DIo91mWFWF6LjNP8IzC4").getBody() // group4
];
var outIds = [
  "10n2kFTdX2TBbDoZoLa8O7B7foLrK7jths83xq6Ic4gw",
  "15HouZFjFQ3QMO48bE9ROJALR_J245_niA_JSK8N1xNw",
  "1hsQuehW0eB3q_OXZ1g7Sv1KgMmQ9wRX-SH0Y3xB_Ivc",
  "16GUmcL5jtvEl2XwpeH4mt68Zy72qOuZyUp2gvEcWSeA",
  "1jObPaB9-qYFL8dli8XSuUIwLbR-Rwq3KYTbpDcjVh7M",
  "1oHpDwAcWlbWBeYk3-u1gJyuMrxMfOm3zr-7mzwl4v1s",
  "1x0T8KVYDTbiEc_gDzk8T5pG9zR3getx9klA6o9BnEvA",
  "1D7cvPGVOcotYxOAmN9ANlCQ4-4AYZwX10yySifOQqZo",
  "1GHFJZFWiqq91hh7XPYs0kVLaMJG8gggbveqguFinX0c",
  "1P7LV53ch2FLBpAS7vr17J8qK9FvGOgrA6hN7gYjbR3g",
  "1o7RHMcnX5XH1DIMavAWzDDsulIiqR0HdRhy7GrMyY7E",
  "1CDkkROy4mXgvSS6h1UEFTZ_1_TNKHxEeOd-XcQXGFg4",
  "10ldZ5BFQ9yn-IiyGFf6cQOQ8-Nmiy5lXeSCLjtmQZxU",
  "1TyhNx9_xnff5GTuYyB3Jw1qEtZHuc5sZq8P7MW-KHqk",
  "1u50BUr5CPTtShFJoZ4yrV9mBV4CnL_VRtNCMKyi2Kz0",
  "1_AH1fyOrA-79v03LiINgFpCESnuPyiafj6DyUecdYiY",
  "1Yy042dZEaJuStQp4cCO3VTyZgrmWqfEBYQ0bi5KZQtU",
  "1ePuixNUuaIExdx8AIU6p65r6BfvVq-oQJJLo0KXHxb4",
  "13wDa6aPKlAyDs0AW06krFfJTqRharEgxN3BRTjLVs_I",
  "15xVZOE9vySQ5ji8h4qb5RcDwJreOzbGCZppOuVrQ4OU",
  "1UayuA2a8-0aXU-Ewdp1C8RD5EazWQFotNANRmgD4Z8o",
  "16dY_93MyEho-E6mNthm7LXMqr6dinKmW5QD2pE5rwVo",
  "1ouHZbT-lebrOkc8NGe2T9jK3vtW0BBJyu4PTs8LE7AI",
  "1nbITlrUfeerGVjkZyfkJetdGO5xZRGp5klbQiApyoB0",
  "1eqK87dKIdvNFIg4b34EzGJTAM4EIsWfqbgpIykFT0GU",
  "1HGfPgWr6vVdr3kfO-Idjrs9PgSLKlA8dCeOTUCa4RwI",
  "1zY3QLgWiu34FYy3shFXt5Ko5s1N23UiTIAVbmbQfQMI",
  "1F7F9Mia33MR0oZgECgGH28qhp-8HFIZFg8OEorev9js",
  "1fiVqd-E787cPUyTGknKqmy3XDpRsFQ0JmjxluGGyTIk",
  "1ZZIaCpZYDEOcn6QODaGo4nmZXaRVweHo79huT2BHazA",
  "13s7ZC5oDNdVKjsyf4PSeqzxe1ALt4Ey-Svq9dNwv4lQ",
  "12riHcLqY60zAeNDSwQ_n_oxkmh2H861ZFBFdvyBJ1iE",
  "17rTocgmiO2fQUSCX9HEClPduiFTJ0xkGwAXsEMJkFsc",
  "14YQ9jaQXVYjpXa31O5QvjVaShlZcBVwNd3WkAdssP0o",
  "1Q28Q8BIE7lIPezU9bAo2-n1ysRI3cZ7Ycj6wckpbmDU"
];
var schools = [];

// for 2020 only
// data0? = unprocessed data
var avgNumCsPerSchool = 3; // averaged number of contestants per school
var data00 = SpreadsheetApp.openById("1-UEe7pI0xU4WeyETeBDwl3oMkK9Xj-fYWsPPQBcPWUc").getSheets()[0];
var cells00 = data00.getDataRange().getValues(); // buffered
var data01 = SpreadsheetApp.openById("1-UEe7pI0xU4WeyETeBDwl3oMkK9Xj-fYWsPPQBcPWUc").getSheets()[1];
var cells01 = data01.getDataRange().getValues(); // buffered
var venueNumsRows = [18, 18, 18], venueNumsCols = [12, 12, 12]; // default
var myVenueNumsRows = [15, 15, 15], myVenueNumsCols = [12, 12, 12]; // custom, so that each column can sit less contestants
var capacities = [207, 207, 207]; // capacities[0] = Hall, capacities[1] = Exhibition Centre, capacities[2] = Gym;
var sectionTimes = ["下午1:00 – 下午1:15",
                    "下午2:30 – 下午2:45",
                    "下午4:00 – 下午4:15",
                    "上午9:00 – 上午9:15",
                    "上午10:30 – 上午10:45"]; // 5 sections for heat event

function main() {
  Logger.clear();
  
//  data0ToData();
  
  getSchools(); // always enable this
  
//  genCids();
  
//  genSeats(); // repeat this until log is empty, disable once log is empty
  
  // mail merge only after genSeats() succeeds
//  mailMergeInit(); // enable to init mailMerge, continue / do nothing otherwise
//  mailMerge(76); // step size
//  dataToData1();
  
//  dataToData2();
  
//  dataToData3();
}
