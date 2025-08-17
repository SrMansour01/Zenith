#include <SPI.h>
#include <mcp_can.h>

const int SPI_CS_PIN = 53; // Pino 53 como Chip Select
MCP_CAN CAN(SPI_CS_PIN);   // Instância do objeto CAN

void setup() {
  Serial.begin(115200);
  
  // Inicializa o MCP2515 (8MHz, 500kbps)
  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("MCP2515 inicializado com sucesso!");
    CAN.setMode(MCP_NORMAL); // Modo normal de operação
  } else {
    Serial.println("Falha na inicialização do MCP2515!");
    while (1); // Trava se falhar
  }
}

void loop() {}