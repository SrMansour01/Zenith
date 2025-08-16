# 📑 Protocolo CAN – Zenith

Este documento define o **protocolo CAN** utilizado no carrinho autônomo Zenith.  
Ele especifica **IDs, payloads, direção, frequência e função de cada mensagem**, garantindo comunicação correta entre Master, Motor ECU e Servo ECU.

---

## 1️⃣ Estrutura de IDs

- Cada mensagem CAN possui um **ID único** (hexadecimal).  
- IDs menores possuem **prioridade maior** no barramento.  
- Direção:  
  - `M → SM` → Master envia comando para Motor Slave  
  - `M → SS` → Master envia comando para Servo Slave  
  - `SM → M` → Motor Slave envia feedback para Master  
  - `SS → M` → Servo Slave envia feedback para Master  
  - `M → All` → Master envia heartbeat  
  - `All → M` → Slaves enviam heartbeat

---

## 2️⃣ Tabela de Mensagens

| ID (hex) | Direção | Nome                     | Período | Descrição |
|----------|---------|--------------------------|---------|-----------|
| 0x100    | M → SM  | Comando Velocidade Motor | 20 ms   | Define a velocidade setpoint do motor (ou de cada motor individual). |
| 0x101    | M → SS  | Comando Ângulo Servo     | 20 ms   | Define o ângulo desejado do servo de direção. |
| 0x110    | SM → M  | Feedback Velocidade Motor | 20 ms  | Retorna velocidade atual do motor e status. |
| 0x111    | SS → M  | Feedback Ângulo Servo    | 20 ms   | Retorna ângulo real do servo. |
| 0x120    | SM → M  | Estado / Faults Motor    | 100 ms  | Indica erros, falhas ou estado do motor. |
| 0x121    | SS → M  | Estado / Faults Servo    | 100 ms  | Indica erros, falhas ou estado do servo. |
| 0x1F0    | M → All | Heartbeat Master         | 100 ms  | Sinal de vida do Master para todos os Slaves. |
| 0x1F1    | All→ M  | Heartbeat Slave          | 100 ms  | Sinal de vida dos Slaves para o Master. |

---

## 3️⃣ Estrutura do Payload

### 3.1 Comando Velocidade Motor (0x100)
| Byte | Descrição         | Tipo   | Observações |
|------|-----------------|--------|-------------|
| 0    | Motor ID         | UInt8  | 1 a 4, caso tenha múltiplos motores |
| 1–2  | Velocidade setpoint | Int16  | Unidades definidas (ex: mm/s ou RPM) |
| 3    | Flags / Reservado | UInt8  | Bits extras para controle especial |
| 4–7  | Reservado        | –      | – |

### 3.2 Comando Ângulo Servo (0x101)
| Byte | Descrição       | Tipo   | Observações |
|------|----------------|--------|-------------|
| 0    | Servo ID        | UInt8  | Normalmente 1, se houver múltiplos servos |
| 1–2  | Ângulo setpoint | Int16  | Ex: -45 a +45 graus |
| 3    | Flags / Reservado | UInt8 | Controle extra ou reserved |
| 4–7  | Reservado      | –      | – |

### 3.3 Feedback Motor (0x110)
| Byte | Descrição          | Tipo   | Observações |
|------|------------------|--------|-------------|
| 0    | Motor ID          | UInt8  | Identificação do motor |
| 1–2  | Velocidade atual  | Int16  | Atual medida pelo encoder |
| 3–4  | Corrente (mA)     | Int16  | Corrente consumida |
| 5    | Temperatura (°C)  | UInt8  | Temperatura do motor |
| 6–7  | Reservado         | –      | – |

### 3.4 Feedback Servo (0x111)
| Byte | Descrição        | Tipo   | Observações |
|------|-----------------|--------|-------------|
| 0    | Servo ID        | UInt8  | – |
| 1–2  | Ângulo atual    | Int16  | Medido pelo encoder ou potenciômetro |
| 3    | Flags / Reservado | UInt8 | – |
| 4–7  | Reservado        | –      | – |

### 3.5 Estado / Faults (0x120 / 0x121)
| Byte | Descrição            | Tipo   | Observações |
|------|--------------------|--------|-------------|
| 0    | ID do módulo         | UInt8  | Motor ou Servo |
| 1    | Código de erro       | UInt8  | 0 = OK, 1 = Overcurrent, 2 = Timeout, etc. |
| 2–7  | Reservado           | –      | – |

### 3.6 Heartbeats (0x1F0 / 0x1F1)
| Byte | Descrição        | Tipo   | Observações |
|------|----------------|--------|-------------|
| 0    | ID do módulo     | UInt8  | Master ou Slave |
| 1    | Estado atual     | UInt8  | READY, ACTIVE, SAFE |
| 2–7  | Reservado       | –      | – |

---

## 4️⃣ Estratégias de Segurança

- **Heartbeat:** detecta falha de comunicação. Se não receber `0x1F0` ou `0x1F1` no tempo limite, o módulo entra em **modo seguro**.  
- **Timeout de comando:** se `0x100` ou `0x101` não chegam dentro de 200–300 ms, o slave para.  
- **Estados:** `INIT → READY → ACTIVE → FAULT → SAFE`.  
- **E-Stop físico:** corta a alimentação do motor independentemente do software.  

---

## 5️⃣ Observações

- Todas as mensagens devem respeitar **máximo de 8 bytes** por frame.  
- Para múltiplos motores, utilize **Motor ID** no payload ou IDs separados no CAN.  
- Heartbeats garantem **robustez e monitoramento contínuo** do barramento.  
- Este protocolo serve como referência para **implementação no Master, Slaves e App React Native**.

---
