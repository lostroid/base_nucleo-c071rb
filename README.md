# base_nucleo-c071rb
!!! English is not my native language, so I use a translator!!! 
  
This project is demo code for configuring operation in 
a bare-metal environment.  

## Development environment  
- STM32CubeIDE  
- NUCLEO-C071RB  
- C Code  
- UART 1Mbps  
   
## We plan to implement the following features  
- ✅ Green LED (LD1) blinks 100ms (https://youtube.com/shorts/IMGCvuJAUY4?si=FYez4cNHNqbf89no)
- ✅ Blue LED (LD2) blinks 500ms
- ✅ ADC CH0
- ✅ ADC CH1
- ✅ UART Debug print  
- ✅ 1.8 TFT-LCD SPI 128x160 (https://youtu.be/QZbChkDfWNI?si=kkYyCHoomGtlhRTD)  
-  1.3 OLED LCD SPI 128x128 
-  USB Device  
   
## The code has the following characteristics  
- It does not use the SysTick interrupt and operates solely based on counter values.  
    
## NUCLEO-C071RB  
<img width="633" height="752" alt="image" src="https://github.com/user-attachments/assets/304b7058-92cb-4b3b-8e99-eda89fcc9b8c" />  
<img width="689" height="788" alt="image" src="https://github.com/user-attachments/assets/48606937-1284-4bd3-b0ef-cb2186ddc125" />  


## ST-LINK PORT UART  
<img width="452" height="442" alt="Image" src="https://github.com/user-attachments/assets/218c6777-365a-4117-aa58-e8b9c24781d8" />   
- Debug print  
<img width="661" height="418" alt="Image" src="https://github.com/user-attachments/assets/9325771a-4b7f-4693-9931-318b5ebf8d05" />  

  
## ✅ 1.8 TFT-LCD SPI 128x160  
- 16Bit RGB, 24Mhz, 32FPS
- Device : ST7735S
<img width="406" height="299" alt="image" src="https://github.com/user-attachments/assets/6251bfa8-7670-4be9-b170-a317d02ca2e9" />   

<img width="640" height="462" alt="image" src="https://github.com/user-attachments/assets/0e45e051-537a-424b-8e60-b062f810ca63" />

## 1.3 OLED LCD SPI 128x128  
<img width="234" height="313" alt="image" src="https://github.com/user-attachments/assets/a0072a9d-681c-4c4b-a3eb-91fb1fe9e7b0" />  

## compilation size is as follows  
<img width="682" height="114" alt="image" src="https://github.com/user-attachments/assets/ba2fbade-76a4-4942-ba81-61587bc6fe54" />  




