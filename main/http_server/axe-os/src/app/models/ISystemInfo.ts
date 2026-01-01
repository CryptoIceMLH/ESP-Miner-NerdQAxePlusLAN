import { eASICModel } from './enum/eASICModel';
import { IHistory } from '../models/IHistory';
import { IStratum } from './IStratum';

export interface ISystemInfo {

    flipscreen: number;
    invertscreen: number;
    autoscreenoff: number;
    power: number,
    maxPower: number,
    minPower: number,
    voltage: number,
    maxVoltage: number,
    minVoltage: number,
    current: number,
    temp: number,
    vrTemp: number,
    hashRateTimestamp: number,
    hashRate: number,
    hashRate_10m: number,
    hashRate_1h: number,
    hashRate_1d: number,
    bestDiff: number,
    bestSessionDiff: number,
    freeHeap: number,
    freeHeapInt: number,
    coreVoltage: number,
    defaultCoreVoltage: number,
    hostname: string,
    hostip: string,
    macAddr: string,
    wifiRSSI: number,
    ssid: string,
    wifiPass: string,
    wifiStatus: string,
    // Ethernet interface fields
    networkMode: string,      // 'wifi' | 'ethernet'
    ethAvailable: number,     // 1 if W5500 detected, 0 otherwise
    ethLinkUp: number,        // 1 if PHY link up, 0 if cable disconnected
    ethConnected: number,     // 1 if has IP address, 0 otherwise
    ethIPv4: string,          // Ethernet IP address (e.g., "192.168.1.121")
    ethMac: string,           // Ethernet MAC address (e.g., "02:00:00:00:00:01")
    sharesAccepted: number,
    sharesRejected: number,
    uptimeSeconds: number,
    asicCount: number,
    smallCoreCount: number,
    ASICModel: eASICModel,
    deviceModel: string,
    stratumURL: string,
    stratumPort: number,
    stratumUser: string,
    stratumEnonceSubscribe: number,
    fallbackStratumURL: string,
    fallbackStratumPort: number,
    fallbackStratumUser: string,
    fallbackStratumEnonceSubscribe: number,
    stratumDifficulty: number,
    poolDifficulty: number,
    frequency: number,
    defaultFrequency: number,
    version: string,
    invertfanpolarity: number,
    autofanspeed: number,
    fanspeed: number,
    manualFanSpeed: number,
    fanrpm: number,
    coreVoltageActual: number,
    lastResetReason: string,
    jobInterval: number,
    lastpingrtt: number,
    recentpingloss: number,
    stratum_keep: number,
    defaultVrFrequency?: number,
    vrFrequency: number,
    shutdown: boolean,

    stratum: IStratum,

    defaultTheme: string,

    boardtemp1?: number,
    boardtemp2?: number,
    overheat_temp: number,

    bdocMode?: boolean,
    bdocOverheatTemp?: number,
    immersionMode?: boolean,

    pidTargetTemp: number,
    pidP: number,
    pidI: number,
    pidD: number,

    asicTemps?: number[],

    history: IHistory

    otp: boolean,
}

// fields swam is using
export interface ISwarmInfo {
    power: number,
    voltage: number,
    temp: number,
    vrTemp: number,
    bestDiff: number,
    bestSessionDiff: number,
    hostname: string,
    hostip: string,
    sharesAccepted: number,
    sharesRejected: number,
    uptimeSeconds: number,
    asicCount: number,
    ASICModel: eASICModel,
    deviceModel: string,
    poolDifficulty: number,
    version: string,
}
