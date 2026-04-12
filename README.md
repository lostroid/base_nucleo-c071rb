# base_nucleo-c071rb
  
이 프로젝트는 STM32CubeIDE 기반의 프로젝트 입니다.  
본 코드는 Baremetal 기반이며 완전 레지스터 구조 입니다.
  
다음과 같은 기능이 구현 되어 있습니다.  
green LED (LD1) 100ms on off    
blue LED (LD2) 500ms on off  
USB debug Print  
  
현재 개발 중이므로 계속 기능이 추가 또는 변경이 될 수 있습니다.    
DBOT(STM32C071G8UX) 기반 코드를 복사 하여 만들었으며  
일부 불필요한 코드가 있을 수 있음.    

현재 DBOT 구현 중이라 이 프로젝트는 지연이 있을수 있습니다.   
단순 동작 구조 학습 용으로 이용 하면 될 것 같습니다.  
  
추후 내부동작구조가 결정이 되면 설명 자료를 작성 예정 (직장인 이라 느림)  
<img width="349" height="429" alt="image" src="https://github.com/user-attachments/assets/b0908237-f395-4812-808c-3c72ade3e329" />

   
USB (VCP)   
<img width="452" height="442" alt="Image" src="https://github.com/user-attachments/assets/218c6777-365a-4117-aa58-e8b9c24781d8" />  
<img width="661" height="418" alt="Image" src="https://github.com/user-attachments/assets/093b6bf0-09a5-4a8b-a563-d20537c2bcdf" />  

컴파일 용량은 다음과 같습니다.  
<img width="686" height="117" alt="image" src="https://github.com/user-attachments/assets/c734e1f0-8d52-490e-ae41-cba9321798b9" />


향후 구현리스트  
1. 1.3 OLED LCD SPI  
<img width="648" height="603" alt="image" src="https://github.com/user-attachments/assets/fc785a91-e53e-49a3-bfc1-81a20135fd61" />

