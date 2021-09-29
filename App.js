import Cowsay from 'react-cowsay';
import {useEffect, useState} from 'react';

function App() {
  const DELAY = 120;
  const NUM_BLINKS = 6;
  const BLINK_NUM_DELAYS = 4;
  const INIT_ROW = 0;
  const INIT_COLUMN = -2;
  const INIT_BLINK = -1;
  
  const STRINGS = [
    'am Kenny.',
    'come from Hong Kong.',
    'am an engineer.',
    'am not a cow.',
    'like playing Tetris.',
  ];
  
  const [row, setRow] = useState(INIT_ROW);
  const [column, setColumn] = useState(INIT_COLUMN);
  const [blink, setBlink] = useState(INIT_BLINK);
  
  const update = () => {
    let tRow = row;
    let tColumn = column;
    let tBlink = blink;
    
    if (tColumn < STRINGS[tRow].length) {
      tColumn = tColumn + 1;
    }
    if (tColumn === STRINGS[tRow].length) {
      tBlink = tBlink + 1;
      if (tBlink === NUM_BLINKS * BLINK_NUM_DELAYS) {
        tColumn = INIT_COLUMN;
        tBlink = INIT_BLINK;
        tRow = tRow + 1;
        if (tRow === STRINGS.length) {
          tRow = INIT_ROW;
        }
      }
    }
    
    // Transact.
    setRow(tRow);
    setColumn(tColumn);
    setBlink(tBlink);
  };
  
  useEffect(() => {
    const interval = setInterval(() => {
      update();
    }, DELAY);
    return () => clearInterval(interval);
  });
  
  const showCursor = () => {
    return column < STRINGS[row].length || Math.floor(blink / BLINK_NUM_DELAYS) % 2 === 0;
  }
  
  return (
    <div className='App'>
      <h1>Kenny Lam's Homepage</h1>
      <Cowsay>{
        'I ' + STRINGS[row].substring(0, column + 1) + (showCursor()
          ? '|'
          : ' ')
      }</Cowsay>
      <p>Hello!</p>
    </div>
  );
}

export default App;
