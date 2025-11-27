#include <WiFi.h>
#include <ESP_Mail_Client.h>

#define WIFI_SSID     "Sebas"
#define WIFI_PASSWORD "24840729"

#define SMTP_HOST "smtp.montevideo.com.uy"
#define SMTP_PORT 465

#define AUTHOR_EMAIL    "mrpo@montevideo.com.uy"
#define AUTHOR_PASSWORD "4017"

#define RECIPIENT_EMAIL "noemida377@gmail.com"
#define RECIPIENT_EMAIL_2 "sprolo81@gmail.com"


SMTPSession smtp;
ESP_Mail_Session session;
SMTP_Message message;

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(250);
  }
  Serial.println("\nWiFi Conectado!");

  sendEmail();
}

void sendEmail() {

  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;

  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;

  // ⚠ PARA ESP MAIL CLIENT 2.3.6: NO TOCAR CERTIFICADOS
  session.secure.startTLS = false;  // Puerto 465 → SSL directo

  message.sender.email = AUTHOR_EMAIL;
  message.sender.name = "ESP32-C3";

  message.subject = "Boton ESP32C3";
  message.addRecipient("Noe", RECIPIENT_EMAIL);
  message.addRecipient("Sebastian", RECIPIENT_EMAIL_2);
 
  message.text.content = "Correo enviado para solicitar ayuda.Llamar a 094060261";

  if (!smtp.connect(&session)) {
    Serial.println("Error de conexión SMTP");
    return;
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.print("Error al enviar: ");
    Serial.println(smtp.errorReason());
  } else {
    Serial.println("Correo enviado!");
  }

  smtp.closeSession();
}

void loop() {}









