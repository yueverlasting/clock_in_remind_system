# clock_in_remind_system
# 已結案
## 詳細規格文件待補

## 上下班打卡提醒裝置

時段: 6am - 10am , 5pm-11pm

需求 : 上下班時段，當人經過，響聲音提醒打卡

開發任務 : 接收訊號、讓喇吧響、時間校正、設定時間區段

器材:NODE MCU 、PIR人體紅外線感測器、8歐喇叭、LED

#### 接線

    PIR OUT腳 => D2
    喇叭      => D5

### 於 5/4 初版上線
    
    檔案 :  all_2.ino 

### 於 5/8 二版上線

    檔案      : all_3.ino 
    修正部分  :  時間寫法 修正為 millis() 數學運算 寫法 &  上班鈴聲錯誤
	
### 於 5/9 三版上線

    檔案      : all_4.ino 
    修正部分  :  修正過一天後時間錯誤 + 去除外接LED 改為板上的預設LED
	
### 播mp3版

	參考資料: https://github.com/earlephilhower/ESP8266Audio
	
### 於 5/9 三版上線

    檔案      : all_5.ino 
    修正部分  :  增設LCD顯示器 用以確認時間是否正確
	PIR OUT腳 => D4
    喇叭      => D5
	LCD-SDA	  => D2
	LCD-SCL	  => D1

### 於 5/17 人聲初版上線

    檔案      : nodemcu_mp3.ino
    播放mp3   需接放大器 電晶體會過熱問題待解決
 
 ### 5/18 人聲二版上線
	
	修正部分 : 電晶體 C端 接一2.2 uF 接地濾波
	
	目前問題 : 仍有雜訊、嘗試關閉wifi or 換成大一點點的電容再弄共鳴音箱
	未來可能 : 5v的電源座 & 線路過亂容易有雜訊
	
 ### 5/21 人聲三版上線
 
	修正部分 : 因線路規劃問題 & wifi未關閉導致的 RX持續送雜訊
	目前仍有wifi對時時仍有雜訊，但不影響正常使用
	
 ###  5/23 終版完成
	
	NPN E : GND
		B : RX
		C : 喇叭一端
	喇叭另一端 5V
	PIR OUT腳 => D4
	LCD-SDA	  => D2
	LCD-SCL	  => D1
	
	修改播放的聲音 & 組合成一盒
