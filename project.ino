#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
#include <SPI.h>
#include<EEPROM.h>
LiquidCrystal_I2C lcd(0X27,16,2); //SCL A5 SDA A4
MFRC522 mfrc522(10, 9); // MFRC522 mfrc522(SS_PIN, RST_PIN)
const int ROW_NUM = 4;// 4 hàng
const int COLUMN_NUM = 3; // 3 cột
#define BUZZER 7 // coi găn chân 7
Servo myservo;
int pos=0;
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
 
byte pin_rows[ROW_NUM] = {A0, A1, A2, A3};
byte pin_column[COLUMN_NUM] = {4, 3, 2};
 
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
char gtlen,gtxuong,gtmenu,gtback; 
int congtru_tong=0,demtong=0;
uint8_t congtru_menu1 = 0; int congtru_menu2 = 0;
int demback = 0;
int demmenu1 = 0; int demmenu2 = 0;
String tagfullID[10];
boolean RFIDMode=true;
char key_pressed = 0; // Variable to store incoming keys
byte initial_password[4];  // Variable to store initial password
byte password[4];   // Variable to store users password
byte password1[4];   // Variable to store users password
uint8_t i = 0,i1=0,a=0,b=0;  // Variable used for counter
boolean pin=false;
boolean menutong=false;
String tagUID = "60 8F 87 21";  // String to store UID of tag. Change it with your tag's UID
String tagID;
boolean tmp=false;
byte id[4];
int tmp1[4];
boolean thoat=false,thoat1=false;
String newpass;
boolean dadoi=false;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(BUZZER, OUTPUT);
noTone(BUZZER);
pinMode(6, OUTPUT);
  myservo.attach(6);
myservo.write(pos);
lcd.init();                    
  lcd.backlight();
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522 
}
void manhinhchinh(){
    lcd.setCursor(0, 0);
    lcd.print("   Door Lock");
    lcd.setCursor(0, 1);
    lcd.print(" Scan Your Tag ");
  }
void menu_tong() // HIỂN THỊ MENU TỔNG
{
  if (congtru_tong == 0)
  {
    lcd.setCursor(0, 0);
    lcd.print(">Them the");
    lcd.setCursor(0, 1);
    lcd.print(" Xoa the");  
  }
  else if (congtru_tong == 1)
  {
    lcd.setCursor(0, 0);
    lcd.print(" Them the");
    lcd.setCursor(0, 1);
    lcd.print(">Xoa the");    
  }
  else if (congtru_tong == 2)
  {
    lcd.setCursor(0, 0);
   lcd.print(" Xoa the");
    lcd.setCursor(0, 1);
    lcd.print(">Doi ma PIN");    
  }
}
void chonmenu_tong() // CHỌN MENU TỔNG
{
  switch (congtru_tong) 
  {
    case 0:
      //Không làm gì
      break;
    case 1:
      //Không làm gì
      break;
  }
}
void chonmenu_1(uint8_t x) // CHỌN MENU 1
{
  lcd.setCursor(0,0);
  lcd.print(">");
  lcd.setCursor(1,0);
  lcd.print(tagfullID[x]);
  if(x==a-1){
    lcd.setCursor(0,1);
  lcd.print("");
    }
  else{
    lcd.setCursor(0,1);
  lcd.print(tagfullID[x+1]);
    }
}
uint8_t soluong(){
    uint8_t sl=0;
       for(uint8_t j=0;j<40;j++){
        if(EEPROM.read(j)!=0)
          sl++;
        }
        return sl/4;
  }
uint8_t getID() {
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return 0;
  }
  tagID = "";
  for(uint8_t j=0;j<4;j++){
    id[j]=0;
    }
  for ( uint8_t j = 0; j < 4; j++) {  // The MIFARE PICCs that we use have 4 byte UID
     id[j] = mfrc522.uid.uidByte[j];
    tagID.concat(String(mfrc522.uid.uidByte[j] < 0x10 ? " 0" : " "));
      tagID.concat(String(mfrc522.uid.uidByte[j], HEX));
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return 1;
}
void right()
{
  tone(BUZZER, 700);
  delay(500);
  noTone(BUZZER); 
}
void wrong()
{
  tone(BUZZER, 300);
  delay(100);
  noTone(BUZZER);
  delay(50);
  tone(BUZZER, 300);
  delay(350);
  noTone(BUZZER);
  delay(1000);
}
void pass(){
   uint8_t idx=0;
    for(uint8_t j=40;j<44;j++){
      initial_password[idx++]=EEPROM.read(j);
      }
    key_pressed = keypad.getKey(); // Storing keys
    if (key_pressed)
    {
      password[i++] = key_pressed-'0'; // Storing in password variable
      lcd.print("*");
    }
    if (i == 4) // If 4 keys are completed
    {
      delay(200);
      int check=0;
      for(uint8_t j=0;j<4;j++){
        if(password[j]==initial_password[j]){
          check++;
          }
        }
      //if (!(strncmp(password, initial_password, 4))) // If password is matched
      if (check==4)
      {
        congtru_tong==0;
        lcd.clear();
        menutong=true;
        i = 0;
        while(menutong){
          char key = keypad.getKey();
    menu_tong();
    if(key=='2'){
      lcd.clear();
  if(demtong==0){
    if(congtru_tong==0){
      congtru_tong=2;
    }
    else{
      congtru_tong--;
    }
  }
}
if(key=='8'){
  lcd.clear();
  if(demtong==0){
    if(congtru_tong==2){
      congtru_tong=0;
    }
    else{
      congtru_tong++;
    }
  } 
}
if(key=='#'){
  congtru_tong=0;
  RFIDMode=true;
  break;
  }
  if(key=='*'&&congtru_tong==0){
    lcd.clear();
    while(1){
      char key1=keypad.getKey();
      thoat=false;
      lcd.setCursor(0, 0);
    lcd.print("Quet the:");
    int tmp=getID();
    if(tmp){
      lcd.clear();
      break;
      }
      if(key1=='#'){
        lcd.clear();
        thoat=true;
        break;
        }
      }
      if(!thoat){
    while(1){
      char key1=keypad.getKey();
      lcd.setCursor(0,0);
      lcd.print("UID:");
      lcd.setCursor(4,0);
      lcd.print(tagID);
      lcd.setCursor(0,1);
      lcd.print("Nhan * de them");
      if(key1=='*'){
        for(uint8_t j=0;j<40;j+=4){
          uint8_t max=j+4,idx=0;
          for(uint8_t k=j;k<max;k++){
            if(EEPROM.read(k)==id[idx++]){
              lcd.clear();
              lcd.print("Da ton tai");
              delay(2000);
              tmp=true;
              break;
              }
            }
            if(tmp){
              break;
              }
          }
        if(tmp){
          tmp=false;
          break;
          }  
        else{
          uint8_t vt=0,l=0;
        while(EEPROM.read(vt)!=0){
          vt++;
          }
          for(uint8_t k=vt;k<vt+4;k++){
            EEPROM.write(k,id[l++]);
            delay(100);
            }   
          lcd.clear();
          lcd.print("Da them");
          delay(2000);
        break;
        }
        }
       if(key1=='#'){
        break;
        }
      }
  }
    lcd.clear();
    }
     if(key=='*'&&congtru_tong==1){
      a=soluong();
      uint8_t idx1=0;
      b=0;
      while( idx1<40){
        if(EEPROM.read(idx1)!=0){
          String tagID1="";
          uint8_t max1=idx1+4; 
          for(uint8_t j=idx1;j<max1;j++){
            tagID1.concat(String(EEPROM.read(j) < 0x10 ? " 0" : " "));
            tagID1.concat(String(EEPROM.read(j), HEX));
            }
            tagID1.toUpperCase();
            tagfullID[b++]=tagID1;
            idx1+=4;
          }
          else
          idx1++;
        }  
     if(a==0){
        lcd.clear();
        lcd.print("Danh sach trong");
        delay(2000);
        lcd.clear();
      }
      else{
        lcd.clear();
          while(1){
      chonmenu_1(congtru_menu1);
      char key2=keypad.getKey();
      if(key2=='2'){
        lcd.clear();
        if(congtru_menu1==0){
          congtru_menu1=a-1;
          }
        else{
          congtru_menu1--;
          }  
        }
        if(key2=='8'){
        lcd.clear();
        if(congtru_menu1==a-1){
          congtru_menu1=0;
          }
        else{
          congtru_menu1++;
          }
          
        }
        if(key2=='#'){
          lcd.clear();
          break;
          }
        if(key2=='*'){
          lcd.clear();
          int idx2=0;
          for(uint8_t j=1;j<=10;j+=3){
            uint8_t mu=1,tong=1;
            for(uint8_t k=j;k<j+2;k++){
              if(tagfullID[congtru_menu1][k]>=65&&tagfullID[congtru_menu1][k]<=70){
                tong+=(((tagfullID[congtru_menu1][k])-'7')*pow(16,mu));
                }
                else{
                tong+=(((tagfullID[congtru_menu1][k])-'0')*pow(16,mu));
                  }
                  mu--;
              }
              tmp1[idx2++]=tong;
            }
            uint8_t timthay=0;
            int idx3;
            for(uint8_t j=0;j<40;j++){
              if(EEPROM.read(j)==tmp1[0]){
                idx3=j;
                timthay=0;
                for(uint8_t k=j+1,l=1;k<j+3,l<4;k++,l++){
                  if(EEPROM.read(k)!=tmp1[l]){
                    break;
                    }
                    else{
                      timthay++;
                      }
                  }
                  if(timthay==3){
                    break;
                    }
                }
              }
              if(timthay==3){
                while(1){
                  char key3=keypad.getKey();
                  lcd.setCursor(0,0);
                  lcd.print("Nhan * de xoa");
                  if(key3=='*'){
                    for(uint8_t j=idx3;j<idx3+4;j++){
                    EEPROM.write(j,0);
                    delay(100);
                  }
                  lcd.clear();
                  lcd.print("Da xoa");
                  delay(2000);
                  break;
                    }
                    if(key3=='#'){
                      lcd.clear();
                      break;
                      }
                  } 
                }
          }
      }
        }
    }
    if(key=='*'&&congtru_tong==2){
      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Nhap mat khau:");
      lcd.setCursor(0,1);
      i1=0;
      newpass="";
      while(1){
        char key4=keypad.getKey();
        if (key4){
      newpass.concat(key4);
      password1[i1++] = key4-'0'; // Storing in password variable
      lcd.print(key4);
      }
      Serial.print(i1);
      if(i1==4){
        lcd.clear();
        while(1){
        char key5=keypad.getKey();
        lcd.setCursor(0,0);
        lcd.print("New pass:");
        lcd.setCursor(9,0);
        lcd.print(newpass);
        lcd.setCursor(0,1);
        lcd.print("Nhan * de doi");
        if(key5=='*'){
          for(uint8_t j=40,k=0;j<44,k<4;j++,k++){
            EEPROM.write(j,password1[k]);
            delay(100);
            }
            lcd.clear();
            lcd.print("Da doi ma PIN");
            delay(2000);
            lcd.clear();
            dadoi=true;
          }
          if(key5=='#'){
            lcd.clear();
          thoat1=true;
          break;
          }
          if(dadoi){
            break;
            }
        }
        }
        if(dadoi){
          break;
          }
          if(thoat1){
            thoat1=false;
            break;
            }
        }
      }
      if(dadoi){
        RFIDMode=true;
        dadoi=false;
        congtru_tong=0;
        break;
        
        }  
    }
        //RFIDMode = true; // Make RFID mode true
        pin=false;
        
      }
      else    // If password is not matched
      {
        lcd.clear();
        lcd.print("Wrong Password");
        delay(3000);
        lcd.clear();
        i = 0;
        RFIDMode = true;  // Make RFID mode true
        pin=false;
      }
    }
}
void loop(){
  char key = keypad.getKey();
  
  if(RFIDMode){
    manhinhchinh();
    int tmp1=getID();
    if(tmp1){
    for(uint8_t j=0;j<40;j+=4){
      int check1=0;
          for(uint8_t k=j,l=0;k<j+4,l<4;k++,l++){
            if(EEPROM.read(k)==id[l]){
              check1++;
              }
            }
            if(check1==4&&tmp1==1){
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("    Xin chao    ");
              right();
              for (pos = 0; pos <=180; pos +=1) {
                  myservo.write(pos);
                  delay(5);
              }
              delay(5000);                   // Thời gian mở cửa
              for (pos = 180; pos >= 0; pos -=1) {
                  myservo.write(pos);
                  delay(5);
              }
              lcd.clear();
              tmp1=0;
              }
              if(check1!=4&&tmp1==1){
                lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("   Xin thu lai  ");
                wrong();
                delay(2000);
                lcd.clear();
                tmp1=0;
                }
          }
  }
  }
  if(key=='*'&&RFIDMode==true){
    pin=true;
    lcd.clear();
    lcd.print("Nhap ma PIN:");
    lcd.setCursor(0, 1);
    RFIDMode=false;
  }
  if(pin){        
        pass();
  }
  
    
}
