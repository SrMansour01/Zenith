 # 🚗 Zenith – CAN Bus Architecture

Este projeto implementa um carrinho autônomo com visão computacional, inspirado na arquitetura modular usada em veículos Mercedes-Benz.  
A ideia central é dividir funções em **ECUs (Electronic Control Units)** que se comunicam via **rede CAN** com um **Master** que toma decisões de alto nível.

---

## 📡 Arquitetura Geral

### Visão Computacional
- Captura vídeo e processa imagens para tomada de decisão (OpenCV).  
- Calcula setpoints de velocidade e ângulo.  
- Pode enviar feed de vídeo em tempo real para o app React Native via **WebSocket ou MJPEG**.  

### Master
- Recebe dados da visão computacional e do app React Native.  
- Combina controle automático + manual (do app).  
- Envia comandos de velocidade e ângulo para os slaves via CAN.  
- Recebe feedback de motores e servo, e envia ao app.  

### Slave – Motor ECU
- Controla o motor DC/BLDC.  
- Lê encoder de velocidade.  
- Implementa PID local de velocidade.  
- Monitora corrente, tensão e temperatura.  
- Reporta feedback ao Master.  

### Slave – Servo ECU
- Controla o servo da direção.  
- Implementa controle de posição/ângulo local.  
- Limita velocidade de esterçamento.  
- Reporta feedback ao Master.  

---

## 🛠️ Topologia da Rede CAN

- **Velocidade:** 500 kbps  
- **Topologia:** Barramento com resistores de 120 Ω em ambas as extremidades  
- **Nós:** Master, ECU Motor, ECU Servo  
- **Transceiver:** MCP2515 + TJA1050 (ou CAN nativo em ESP32/Teensy)  
- **Cabo:** Par trançado para CANH / CANL  

---

## 📑 Protocolo CAN

Cada mensagem CAN tem um **ID único** e payload de até 8 bytes.  
A comunicação segue o padrão automotivo: **mensagens periódicas e broadcast**.

### IDs principais

| ID (hex) | Direção | Nome                       | Período |
|---------:|:-------:|----------------------------|:-------:|
| 0x100    | M → SM  | Comando Velocidade Motor   | 20 ms   |
| 0x101    | M → SS  | Comando Ângulo Servo       | 20 ms   |
| 0x110    | SM → M  | Feedback Velocidade Motor  | 20 ms   |
| 0x111    | SS → M  | Feedback Ângulo Servo      | 20 ms   |
| 0x120    | SM → M  | Estado / Faults Motor      | 100 ms  |
| 0x121    | SS → M  | Estado / Faults Servo      | 100 ms  |
| 0x1F0    | M → All | Heartbeat Master           | 100 ms  |
| 0x1F1    | All→ M  | Heartbeat Slave            | 100 ms  |

---

## ⚠️ Estratégias de Segurança

### Heartbeat
- Se o Master não envia `0x1F0` por >300 ms → Slaves entram em **modo seguro**.  
- Motor rampa para 0 mm/s.  
- Servo volta ao centro gradualmente.

### Timeout de comando
- Se `0x100/0x101` não chegam em 200–300 ms → Slaves assumem falha e param.

### Estados
- `INIT → READY → ACTIVE → FAULT → SAFE`

### E-Stop físico
- Botão corta a alimentação do motor (independente do software).

---

## 📱 Controle via React Native

- O app permite controlar **velocidade e ângulo manualmente**.  
- Recebe **feedback em tempo real** de motores e servo, além do **vídeo da visão computacional**.  
- Comunicação com Python Master via **Wi-Fi (WebSocket/TCP) ou Bluetooth**.  

### Exemplo de mensagem JSON enviada pelo app
```json
{
  "velocidade": 1200,
  "angulo": 45
}
