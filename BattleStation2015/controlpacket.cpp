#include "controlpacket.h"

ControlPacket::ControlPacket() {
    this->data = QByteArray(PACKET_SIZE, 0x00);

    reset();
}

ControlPacket::~ControlPacket() {
}

//void ControlPacket::setThrusters(Motor** motors) {
//    for (int i = 0; i <THRUSTER_COUNT; i++) {
//        if (motors[i]) {
//            setThruster(motors[i]);
//        }
//    }
//}

void ControlPacket::setThruster(int address, quint8 value) {
    if (address < 1 || address > 8) return;

    thrusterValues[address-1] = value;
}

//void ControlPacket::setThruster(Motor* motor) {
//    if (motor == NULL) return;

//    quint8 value = (motor->getSpeed() >> 1);
//    if (motor->getDirection()) {
//        value = value & 0x80;
//    }

//    if (motor->getAddress() >= 1 && motor->getAddress() <= 8) {
//        int index = motor->getAddress() - 1;
//        thrusterValues[index] = value;
//    }
//}

void ControlPacket::setFootTurner(quint8 value) {
   footTurner = value;
}

void ControlPacket::setCamMux1(bool camMux1) {
    this->camMux1 = camMux1;
}

void ControlPacket::setCamMux2(bool camMux2) {
    this->camMux2 = camMux2;
}

void ControlPacket::setBilgePump(bool enabled) {
    bilgePump = enabled;
}

void ControlPacket::setVoltageMeasurement(bool enabled) {
    voltageMeasurement = enabled;
}

void ControlPacket::setLaser(bool enabled) {
    laser = enabled;
}

void ControlPacket::setClaw(bool state) {
    clawClose = state;
}

void ControlPacket::setLEDs(quint8 values[]) {
    for (int i = 0; i < LED_COUNT; i++) {
        ledValues[i] = values[i];
    }
}

void ControlPacket::setVStepper(bool upDirection, quint8 value) {
    if (value > 7) {
        vStepperAmount = 0;
        return;
    }

    vStepperAmount = value;
    vStepperUpDirection = upDirection;

}

void ControlPacket::setHStepper(bool rightDirection, quint8 value) {
    if (value > 7) {
        hStepperAmount = 0;
        return;
    }

    hStepperAmount = value;
    hStepperRightDirection = rightDirection;
}

quint8 ControlPacket::getStepperByte() {
    quint8 stepperByte = 0x00;

    if (hStepperRightDirection) stepperByte |= 0x80;
    if (vStepperUpDirection) stepperByte |= 0x08;

    stepperByte |= (hStepperAmount << 4);
    stepperByte |= (vStepperAmount & 0x0F);

    return stepperByte;
}

quint8 ControlPacket::getToolByte() {
    quint8 toolByte = 0x00;

    if (camMux1)            toolByte |= 0x80;     //1
    if (camMux2)            toolByte |= 0x40;     //2
    if (clawClose)          toolByte |= 0x20;     //3
    //stepper 0             toolByte |= 0x10;     //4
    if (bilgePump)          toolByte |= 0x08;     //5
    //N/A                   toolByte |= 0x04;     //6
    if (voltageMeasurement) toolByte |= 0x02;     //7
    if (laser)              toolByte |= 0x01;     //8


    return toolByte;
}

quint8 ControlPacket::crc(QByteArray data) {
    quint8 crc = 0;
    int size = data.size();

    for (int i = 1; i < size-2; ++i) {

        quint8 inbyte = data.at(i);

        for (int i = 8; i; i--) {
            quint8 mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix) crc ^= 0xD5;
            inbyte >>= 1;
        }
    }

    return crc;
}

void ControlPacket::assemblePacket() {
    data[0] = HEADER;

    for (int i = 0; i < THRUSTER_COUNT; i++) {
        data[THRUSTER_BYTE_START+i] = thrusterValues[i];
    }

    data[FOOT_TURNER] = footTurner;

    data[TOOLS] = getToolByte();

    data[STEPPER] = getStepperByte();

    for (int i = 0; i < LED_COUNT; i++) {
        data[LED_START+i] = ledValues[i];
    }

    data[PACKET_SIZE-3] = CONTROL;
    data[PACKET_SIZE-2] = CRC_BYTE;
    data[PACKET_SIZE-1] = TAIL;

    data[PACKET_SIZE-2] = crc(data);
}

QByteArray ControlPacket::getPacket() {
    assemblePacket();
    return data;
}

void ControlPacket::reset() {
    data[0] = HEADER;

    data[PACKET_SIZE-3] = CONTROL;
    data[PACKET_SIZE-2] = CRC_BYTE;
    data[PACKET_SIZE-1] = TAIL;

    for (int i = 0; i < THRUSTER_COUNT; i++) {
        thrusterValues[i] = 0;
    }

    footTurner = 0;

    camMux1 = false;
    camMux2 = false;

    bilgePump = false;

    voltageMeasurement = false;

    laser = false;

    clawClose = false;

    vStepperUpDirection = true;
    vStepperAmount = 0;

    hStepperRightDirection = true;
    hStepperAmount = 0;

    for (int i = 0; i < LED_COUNT; i++) {
        ledValues[i] = 0;
    }
}

void ControlPacket::print() {
    assemblePacket();
    int size = data.size();
    qDebug("Size: %d", size);
    for (int i = 0; i < size; ++i) {
        qint8 num = (data[i] & 0x80) ? (-101*(0x7F & data[i]))/128 : (101*(0x7F & data[i]))/128;
        qDebug("[%d]: [%d]", (quint8) data.at(i), num);
    }
}

