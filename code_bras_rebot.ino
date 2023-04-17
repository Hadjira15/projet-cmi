#include <SoftwareSerial.h>
#include <Pixy2.h>
//Variables
char couleur;
int resultat = 1;
bool objet_trouve;
Pixy2 pixy;
// Définition des broches utilisées pour la communication série
SoftwareSerial mySerial(7, 8); // RX, TX
SoftwareSerial BTSerial(A0, A1); // RX | TX


void setup() {
  //Init Serial USB
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  Serial.begin(9600);
  mySerial.begin(9600);
  //Init Pixy2
  pixy.init();
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
   BTSerial.begin(9600);
}

void loop() {

  if (BTSerial.available())
  {
    couleur =BTSerial.read(); //lecture de la commande envoyée depuis l'application de contrôle Bluetooth
    delay(10);// ajouter un court délai pour éviter les lectures en double
    resultat = signa(couleur);
    if (!objet_trouve) { // si l'objet n'a pas été trouvé
      objet_trouve = mouvement(resultat); // lancement de la fonction mouvement pour chercher l'objet
      digitalWrite(2, HIGH);
      delay(100);
      tourner_base(2500);
      tourner_epaule(1500);
      tourner_coude(2000);
      tourner_poignet(700);
      digitalWrite(2, LOW);
    } else {
      // si l'objet a été trouvé
       positionInitiale();
       objet_trouve=false;
    }
  }
}

void positionInitiale() {
  tourner_base(2500);
  tourner_epaule(1500);
  tourner_coude(2000);
  tourner_poignet(700);
  delay(1000);
}
int signa(char couleur)
{
  int signa_attendu;

  if (couleur == 'R')
  {
    signa_attendu = 2;
  }
  else if (couleur == 'V') {
    signa_attendu = 1;
  }
  else if (couleur == 'B') {
    signa_attendu = 3;
  }
  else if (couleur == 'J') {
    signa_attendu = 4;
  }
  return (signa_attendu);
}


bool mouvement(int n) {
  bool objet_trouve = false;
  pixy.ccc.getBlocks();
  int signature = 0;
  // s'il ya des objets détéctés
  if (pixy.ccc.numBlocks) {

    for (int i = 0; i < pixy.ccc.numBlocks; i++) {
      signature = pixy.ccc.blocks[i].m_signature;
      if (signature == n)
      {
        objet_trouve = true;
        double x = pixy.ccc.blocks[i].m_x;
        double y = pixy.ccc.blocks[i].m_y;
        Serial.print("la cordonnéé x est : ");
        Serial.println(  x);
        Serial.print("la cordonnée y est : ");
        Serial.println( y);

        x = x;//changement de repere
        y = y + 15;

        Serial.print("la NV cordonnéé x est : ");
        Serial.println(  x);
        Serial.print("la NV cordonnée y est : ");
        Serial.println( y);

        double rapport = (310.0 / 47.0);//pour convertir du cm au pixel
        Serial.print("la valeur du rapport est : ");
        Serial.println( rapport);
        // y = y * (47.0f / 310.0f);
        //x = x * (47.0f / 310.0f);
        double h = 15 * rapport;

        Serial.print("la valeur de h est : ");
        Serial.println( h);

        double hauteur = h + y;//la distance entre la base et l'objet
        double valeur = 0;
        double angle = 0;
        double angleRadion = 0;

        if (x < 23 * rapport)
        {
          Serial.print("La valeur de x est inferieure a 23 ");
          valeur = ((23 * rapport) - x) / hauteur;
          Serial.print("valeur if one=  " );
          Serial.println(valeur);


          angleRadion = atan(valeur);
          Serial.print("angleRadion if one =  " );
          Serial.println(angleRadion);
          angle = degrees(angleRadion);
          Serial.print("angle if one =  " );
          Serial.println(angle);


          angle = 180 - (90 - angle );
          Serial.print("new if one valeur =  " );
          Serial.println(angle);

        }
        else {
          Serial.println("La valeur de x est superieure a 23 ");

          valeur = (x - (23 * rapport)) / hauteur;
          Serial.print("valeur if two=  " );
          Serial.println(valeur);

          angleRadion = atan(valeur);
          Serial.print("angleRadion if two =  " );
          Serial.println(angleRadion);
          angle = degrees(angleRadion);

          Serial.print("angle if two =  " );
          Serial.println(angle);
          angle = 180 - ( 90 + angle) ;
          Serial.print("new angle if two =  " );
          Serial.println(angle);
        }
        Serial.print("l'angle a est : ");
        Serial.println(angle);
        int angleservo = map(angle, 0, 180, 500, 2500);
        Serial.println(angleservo);
        tourner_base(angleservo);
        delay(2000);
        // ici on a termine avec la partie qui pilote la base et on s'interesseà l'epaule et le coude
        if (y > 6 && y < 10)
        {
          tourner_epaule(1250);
          tourner_coude(2000);
          tourner_poignet(1050);
        }
        else if (y > 11 && y < 38) {
          tourner_epaule(1100);
          tourner_coude(1700);
          tourner_poignet(900);
        }
        else if (y > 39 && y < 81) {
          tourner_coude(1400);
           tourner_epaule(950);
          tourner_poignet(800);
        }

        else if (y > 82 && y < 112) {
          tourner_epaule(800);
          tourner_coude(1000);
          tourner_poignet(800);
        }
      }
    }
  }
  return objet_trouve;
}

void tourner_base(int angle) {
  if (angle < 500)
  {
    angle = 500;
  }
  if (angle > 2500)
  {
    angle = 2500;
  }
  mySerial.print("#0P");
  mySerial.print(angle);
  mySerial.println("T1000");
  delay(2000);
}

void tourner_epaule(int angle) {
  if (angle < 500)
  {
    angle = 500;
  }
  if (angle > 2500)
  {
    angle = 2500;
  }
  mySerial.print("#1P");
  mySerial.print(angle);
  mySerial.println("T1000");
  delay(2000);
}
void tourner_coude(int angle) {
  if (angle < 500)
  {
    angle = 500;
  }
  if (angle > 2500)
  {
    angle = 2500;
  }
  mySerial.print("#2P");
  mySerial.print(angle);
  mySerial.println("T1000");
  delay(2000);
}

void tourner_poignet(int angle) {
  if (angle < 500)
  {
    angle = 500;
  }
  if (angle > 2500)
  {
    angle = 2500;
  }
  mySerial.print("#3P");
  mySerial.print(angle);
  mySerial.println("T1000");
  delay(2000);
}
