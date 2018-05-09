# clock_in_remind_system

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
	
	
