# iotsword  

## 概要  

- 光る剣  
- 無線でスマホから操作できる  
- ぴかぴかしてかっこいい  
- microUSBで給電(max 500mA)  

## 機能  

- 流れるLEDは7種類のパターン/ランダムから選択可能*  
- その他の固定LEDは任意の色に光らせられる*  
- ボタン長押しで一時的に全て消える  
\*スマホなどからブラウザで操作可能  

## 材料  

- 木とダンボール  
- ledストリップ(WS2812B)  
- esp32  
- 材料費は3000円くらいかな？  

## どうやってうごいてんの?  

- Wi-Fiのアクセスポイント+webサーバー  
- ネットに繋がってないLAN  
- パターン選択はXHR(ajaxの友達)でリクエストを飛ばしている  
- 意色の部分はクエリパラメータ  
- ArduinoIDEでごりごりHTML,CSS,JSを書いた  
- インターネットに繋がってないので実はIoTじゃない(？)  
