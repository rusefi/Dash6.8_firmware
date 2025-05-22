/*
 * ErrorCode.c
 *
 *  Created on: Feb 13, 2025
 *      Author: Раман
 */

#include "ErrorCode.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>

const char* Error_code(uint16_t code) {


    switch (code) {
        case 0:
            return "None";

            // OBD2 коды неисправности P000 + цифра из кейса

        case 90:
            return "Fuel Pressure Sensor Missing";
        case 100:
            return "Mass or Volume Air Flow Circuit Malfunction";
        case 105:
            return "Manifold Absolute Pressure Circuit Malfunction";
        case 106:
            return "MAP Timeout";
        case 107:
            return "MAP Low";
        case 108:
            return "MAP High";
        case 111:
            return "ThermistorConfig";
        case 110:
            return "IAT Timeout";
        case 112:
            return "IAT Low";
        case 113:
            return "IAT High";
        case 115:
            return "CLT Timeout";
        case 117:
            return "CLT Low";
        case 118:
            return "CLT High";
        case 121:
            return "TPS Configuration";
        case 120:
            return "TPS1 Primary Timeout";
        case 122:
            return "TPS1 Primary Low";
        case 123:
            return "TPS1 Primary High";
        case 176:
            return "FlexSensor Timeout";
        case 178:
            return "FlexSensor Low";
        case 179:
            return "FlexSensor High";
        case 201:
            return "Injector Circuit 1";
        case 202:
            return "Injector Circuit 2";
        case 203:
            return "Injector Circuit 3";
        case 204:
            return "Injector Circuit 4";
        case 205:
            return "Injector Circuit 5";
        case 206:
            return "Injector Circuit 6";
        case 207:
            return "Injector Circuit 7";
        case 208:
            return "Injector Circuit 8";
        case 209:
            return "Injector Circuit 9";
        case 210:
            return "Injector Circuit 10";
        case 211:
            return "Injector Circuit 11";
        case 212:
            return "Injector Circuit 12";
        case 220:
            return "TPS1 Secondary Timeout";
        case 222:
            return "TPS1 Secondary Low";
        case 223:
            return "TPS1 Secondary High";
        case 225:
            return "TPS2 Primary Timeout";
        case 227:
            return "TPS2 Primary Low";
        case 228:
            return "TPS2 Primary High";
        case 261:
            return "Injector Circuit 1 Low";
        case 262:
            return "Injector Circuit 1 High";
        case 335:
            return "Crankshaft Position Sensor A Circuit Malfunction";
        case 341:
            return "Camshaft Position Sensor Circuit Range Performance";
        case 351:
            return "Ignition Circuit 1";
        case 352:
            return "Ignition Circuit 2";
        case 353:
            return "Ignition Circuit 3";
        case 354:
            return "Ignition Circuit 4";
        case 355:
            return "Ignition Circuit 5";
        case 356:
            return "Ignition Circuit 6";
        case 357:
            return "Ignition Circuit 7";
        case 358:
            return "Ignition Circuit 8";
        case 359:
            return "Ignition Circuit 9";
        case 360:
            return "Ignition Circuit 10";
        case 361:
            return "Ignition Circuit 11";
        case 362:
            return "Ignition Circuit 12";
        case 520:
            return "Oil Pressure Sensor Malfunction";
        case 560:
            return "System Voltage Malfunction";
        case 606:
            return "PCM Processor Fault";
        case 638:
            return "Throttle Actuator Control Range Performance Bank 1";
        case 2120:
            return "TPS2 Secondary Timeout";
        case 2122:
            return "TPS2 Secondary Low";
        case 2123:
            return "TPS2 Secondary High";
        case 2125:
            return "PPS Primary Timeout";
        case 2127:
            return "PPS Primary Low";
        case 2128:
            return "PPS Primary High";
        case 2130:
            return "PPS Secondary Timeout";
        case 2132:
            return "PPS Secondary Low";
        case 2133:
            return "PPS Secondary High";
        case 2135:
            return "TPS1 Correlation";
        case 2136:
            return "TPS2 Correlation";
        case 2137:
            return "PPS Correlation";
        case 2158:
            return "Vehicle Speed SensorB";
        case 2226:
            return "Barometric Press Circ";
        case 2227:
            return "Barometric Press Circ Range Perf";
        case 2900:
            return "Wideband 1 Fault";
        case 2901:
            return "Wideband 2 Fault";

            // Кастомные коды RUSEFI C

        case 6000:
            return "Nan engine load";
        case 6001:
            return "Wrong algorithm";
        case 6002:
            return "Nan engine load 2";
        case 6003:
            return "6003";
        case 6004:
            return "6004";
        case 6005:
            return "6005";
        case 6006:
            return "6006";
        case 6007:
            return "6007";
        case 6008:
            return "6008";
        case 6009:
            return "6009";
        case 6010:
            return "6010";
        case 6011:
            return "6011";
        case 6012:
            return "Err interpolate 1";
        case 6013:
            return "Err interpolate 2";
        case 6014:
            return "Err interpolate 3";
        case 6015:
            return "Err interpolate 4";
        case 6016:
            return "Param range";
        case 6017:
            return "Maf needed";
        case 6018:
            return "Unknown algorithm";
        case 6019:
            return "Coil pin not assigned";
        case 6020:
            return "Injection no pin assigned";
        case 6021:
            return "Unexpected injection mode";
        case 6022:
            return "6022";
        case 6023:
            return "Unknown firing order";
        case 6024:
            return "Wrong firing order";
        case 6025:
            return "Ignition mode";
        case 6026:
            return "26";
        case 6027:
            return "Unexpected engine type";
        case 6028:
            return "Invalid tps setting";
        case 6030:
            return "Nan injection";
        case 6031:
            return "Neg injection";
        case 6032:
            return "Zero dwell";
        case 6033:
            return "Dwell too long";
        case 6034:
            return "Skipping stroke";
        case 6037:
            return "6037";
        case 6038:
            return "Analog input not configured";
        case 6039:
            return "Wrong adc mode";
        case 6040:
            return "6040";
        case 6041:
            return "Knock processor";
        case 6042:
            return "Local freeze";
        case 6043:
            return "6043";
        case 6044:
            return "Logging buffer overflow";
        case 6045:
            return "6045";
        case 6046:
            return "6046";
        case 6047:
            return "6047";
        case 6048:
            return "Pin conflict";
        case 6049:
            return "Low frequency";
        case 6051:
            return "6051";
        case 6052:
            return "Ts page mismatch";
        case 6053:
            return "Ts output mismatch";
        case 6054:
            return "Too long cranking fuel injection";
        case 6055:
            return "Interpolate nan";
        case 6056:
            return "Histo name error";
        case 6057:
            return "Aux out of order";
        case 6058:
            return "High frequency";
        case 6059:
            return "59";
        case 6060:
            return "Mmc start1";
        case 6061:
            return "Mmc start2";
        case 6062:
            return "62";
        case 6063:
            return "63";
        case 6064:
            return "64";
        case 6065:
            return "65";
        case 6066:
            return "66";
        case 6067:
            return "67";
        case 6068:
            return "68";
        case 6069:
            return "69";
        case 6070:
            return "70";
        case 6071:
            return "71";
        case 6072:
            return "72";
        case 6073:
            return "73";
        case 6074:
            return "74";
        case 6075:
            return "75";
        case 6076:
            return "76";
        case 6077:
            return "77";
        case 6078:
            return "78";
        case 6079:
            return "79";
        case 6081:
            return "81";
        case 6082:
            return "82";
        case 6083:
            return "83";
        case 6084:
            return "84";
        case 6085:
            return "85";
        case 6086:
            return "86";
        case 6087:
            return "87";
        case 6088:
            return "88";
        case 6089:
            return "89";
        case 6090:
            return "90";
        case 6091:
            return "91";
        case 6092:
            return "92";
        case 6093:
            return "93";
        case 6094:
            return "94";
        case 6095:
            return "95";
        case 6096:
            return "96";
        case 6097:
            return "Pid dterm";
        case 6098:
            return "Dwell";
        case 6099:
            return "Ts overflow";
        case 6100:
            return "CUSTOM_ERR_OP_MODE";
        case 6101:
            return "Err trigger zero";
        case 6102:
            return "Err 6102";
        case 6103:
            return "Err 2nd watchdog";
        case 6104:
            return "Err invalid injection mode";
        case 6105:
            return "Err wave 1";
        case 6106:
            return "Err wave 2";
        case 6107:
            return "Runtime critical test error";
        case 6108:
            return "Err ignition mode";
        case 6109:
            return "Err can configuration";
        case 6110:
            return "Err interpolate";
        case 6111:
            return "Err not initialized trigger";
        case 6112:
            return "Err map type";
        case 6113:
            return "Err therm";
        case 6114:
            return "Err natural logarithm error";
        case 6115:
            return "Err looped queue";
        case 6116:
            return "Err pwm 1";
        case 6117:
            return "Err pwm 2";
        case 6118:
            return "Err dwell duration";
        case 6119:
            return "Err no shape";
        case 6121:
            return "Err input during initialisation";
        case 6122:
            return "Err 6122";
        case 6123:
            return "Err 6123";
        case 6124:
            return "Err 6124";
        case 6125:
            return "Err 6125";
        case 6126:
            return "Err 6126";
        case 6127:
            return "Err 6127";
        case 6128:
            return "Err 6128";
        case 6129:
            return "Err 6129";
        case 6130:
            return "Err invalid pin";
        case 6131:
            return "Err 6131";
        case 6132:
            return "Err unknown port";
        case 6133:
            return "Err pin already used 1";
        case 6134:
            return "Err pin already used 2";
        case 6135:
            return "Err 6135";
        case 6136:
            return "Err tcharge not ready";
        case 6137:
            return "Err trigger waveform too long";
        case 6138:
            return "Err fuel table not ready";
        case 6139:
            return "Err tcharge not ready2";
        case 6140:
            return "Err command lower case expected";
        case 6141:
            return "Err flash crc failed";
        case 6142:
            return "Err not input pin";
        case 6143:
            return "Err skipped tooth shape";
        case 6144:
            return "Err unexpected shaft event";
        case 6145:
            return "Err sd mount failed";
        case 6146:
            return "Err sd seek failed";
        case 6147:
            return "Err out of order";
        case 6148:
            return "Err t2 charge";
        case 6500:
            return "Err assert";
        case 6501:
            return "Err assert void";
        case 6502:
            return "Error fl stack overflow";
        case 6503:
            return "6503";
        case 6504:
            return "Flstack";
        case 6505:
            return "Err nan tcharge";
        case 6506:
            return "Ego type";
        case 6507:
            return "List loop";
        case 6508:
            return "Err lock issue";
        case 6509:
            return "Config not ready";
        case 6510:
            return "Err trg angle order";
        case 6511:
            return "Err state null";
        case 6512:
            return "Err same angle";
        case 6513:
            return "Error trigger drama";
        case 6514:
            return "Map angle param";
        case 6515:
            return "Err display mode";
        case 6516:
            return "Err adc unknown channel";
        case 6517:
            return "Err adc used";
        case 6518:
            return "Err adc depth slow";
        case 6519:
            return "Err adc depth fast";
        case 6520:
            return "Err 6520";
        case 6521:
            return "Err 6521";
        case 6522:
            return "Err 6522";
        case 6523:
            return "Err 6523";
        case 6524:
            return "Err unexpected spi";
        case 6525:
            return "Err ext mode";
        case 6526:
            return "Err timer overflow";
        case 6527:
            return "Err 6527";
        case 6528:
            return "Err scheduling error";
        case 6529:
            return "Err logging not ready";
        case 6530:
            return "Error nan find index";
        case 6531:
            return "Error null buffer";
        case 6532:
            return "Err buff init error";
        case 6533:
            return "Err interpolate param";
        case 6534:
            return "Error logging size calc";
        case 6535:
            return "Err adc channel";
        case 6536:
            return "Err angle";
        case 6537:
            return "Err logging null";
        case 6538:
            return "Err parsing error";
        case 6539:
            return "Err injector lag";
        case 6540:
            return "Err axis order";
        case 6541:
            return "Hw timer";
        case 6542:
            return "Inj duration";
        case 6543:
            return "Add base";
        case 6544:
            return "Err 6544";
        case 6545:
            return "Err 6545";
        case 6546:
            return "Unexpected tdc angle";
        case 6547:
            return "Invalid global offset";
        case 6548:
            return "Unexpected map value";
        case 6549:
            return "Err 6549";
        case 6550:
            return "Err 6550";
        case 6551:
            return "Trigger sync angle";
        case 6552:
            return "Trigger sync angle2";
        case 6553:
            return "Err 6553";
        case 6554:
            return "Err 6554";
        case 6555:
            return "Err 6555";
        case 6556:
            return "Err 6556";
        case 6557:
            return "Err 6557";
        case 6558:
            return "Err 6558";
        case 6559:
            return "Trigger sync angle range";
        case 6560:
            return "Err trigger angle range";
        case 6561:
            return "Err 6561";
        case 6562:
            return "Err 6562";
        case 6563:
            return "Err 6563";
        case 6564:
            return "Err 6564";
        case 6565:
            return "Err 6565";
        case 6566:
            return "Err cyl angle";
        case 6567:
            return "Err 6567";
        case 6568:
            return "Err 6568";
        case 6569:
            return "Err 6569";
        case 6570:
            return "Err 6570";
        case 6571:
            return "Err 6571";
        case 6572:
            return "Err array remove";
        case 6573:
            return "Err 6573";
        case 6574:
            return "Err 6574";
        case 6575:
            return "Err 6575";
        case 6576:
            return "Err 6576";
        case 6577:
            return "Err 6577";
        case 6578:
            return "Null engine ptr\nC6578";
        case 6579:
            return "Duty too low\nC6579";
        case 6580:
            return "Err 6580\nC6580";
        case 6581:
            return "Err 6581\nC6581";
        case 6582:
            return "Err 6582\nC6582";
        case 6583:
            return "Err 6583\nC6583";
        case 6584:
            return "Err 6584\nC6584";
        case 6585:
            return "Err 6585\nC6585";
        case 6586:
            return "Err 6586\nC6586";
        case 6587:
            return "Err 6587\nC6587";
        case 6588:
            return "Null shape\nC6588";
        case 6589:
            return "Spark angle 1\nC6589";
        case 6590:
            return "Err 6590\nC6590";
        case 6591:
            return "Err 6591\nC6591";
        case 6592:
            return "Err 6592\nC6592";
        case 6593:
            return "Err 6593\nC6593";
        case 6594:
            return "Shape len zero\nC6594";
        case 6595:
            return "Trigger cycle\nC6595";
        case 6596:
            return "Trigger cycle nan\nC6596";
        case 6597:
            return "Omode undef\nC6597";
        case 6598:
            return "Err 6598\nC6598";
        case 6599:
            return "Err 6599\nC6599";
        case 6600:
            return "Err 6600\nC6600";
        case 6601:
            return "Console too many\nC6601";
        case 6602:
            return "Append null\nC6602";
        case 6603:
            return "Err 6603\nC6603";
        case 6604:
            return "Err 6604\nC6604";
        case 6605:
            return "Err 6605\nC6605";
        case 6606:
            return "Err 6606\nC6606";
        case 6607:
            return "Append stack\nC6607";
        case 6608:
            return "Err 6608\nC6608";
        case 6609:
            return "Err 6609\nC6609";
        case 6610:
            return "Err 6610\nC6610";
        case 6611:
            return "Err 6611\nC6611";
        case 6612:
            return "Err 6612\nC6612";
        case 6613:
            return "Err 6613\nC6613";
        case 6614:
            return "Err 6614\nC6614";
        case 6615:
            return "Err 6615\nC6615";
        case 6616:
            return "Err 6616\nC6616";
        case 6617:
            return "Err timer state\nC6617";
        case 6618:
            return "Err 6618\nC6618";
        case 6619:
            return "Err 6619\nC6619";
        case 6620:
            return "Apply stack\nC6620";
        case 6621:
            return "Err 6621\nC6621";
        case 6622:
            return "Err 6622\nC6622";
        case 6623:
            return "Err 6623\nC6623";
        case 6624:
            return "Err 6624\nC6624";
        case 6625:
            return "Err 6625\nC6625";
        case 6626:
            return "Event 6626\nC6626";
        case 6627:
            return "Stack 6627\nC6627";
        case 6628:
            return "Err 6628\nC6628";
        case 6629:
            return "Stack 6629\nC6629";
        case 6630:
            return "Err 6030\nC6630";
        case 6631:
            return "Err 6631\nC6631";
        case 6632:
            return "Err 6632\nC6632";
        case 6633:
            return "Angle nan\nC6633";
        case 6634:
            return "Err 6634\nC6634";
        case 6635:
            return "Err 6635\nC6635";
        case 6636:
            return "Err 6636\nC6636";
        case 6637:
            return "Conf null\nC6637";
        case 6638:
            return "Trigger event type\nC6638";
        case 6639:
            return "Err 6639\nC6639";
        case 6640:
            return "Trigger unexpected\nC6640";
        case 6641:
            return "Err 6641\nC6641";
        case 6642:
            return "Trigger stack\nC6642";
        case 6643:
            return "Err 6643\nC6643";
        case 6644:
            return "Idle wave cnt\nC6644";
        case 6645:
            return "Err 6645\nC6645";
        case 6646:
            return "Err 6646\nC6646";
        case 6647:
            return "Err 6647\nC6647";
        case 6648:
            return "Err 6648\nC6648";
        case 6649:
            return "Err 6649\nC6649";
        case 6650:
            return "Err 6650\nC6650";
        case 6651:
            return "Err 6651\nC6651";
        case 6652:
            return "Err 6652\nC6652";
        case 6653:
            return "Err 6653\nC6653";
        case 6654:
            return "Err 6654\nC6654";
        case 6655:
            return "Err 6655\nC6655";
        case 6656:
            return "Err 6656\nC6656";
        case 6657:
            return "Err 6657\nC6657";
        case 6658:
            return "Err 6658\nC6658";
        case 6659:
            return "Err 6659\nC6659";
        case 6660:
            return "Err 6660\nC6660";
        case 6661:
            return "Err 6661\nC6661";
        case 6662:
            return "Err 6662\nC6662";
        case 6663:
            return "Err 6663\nC6663";
        case 6664:
            return "Err 6664\nC6664";
        case 6665:
            return "Err 6665\nC6665";
        case 6666:
            return "Err 6666\nC6666";
        case 6667:
            return "Err adcance calc angle\nC6667";
        case 6668:
            return "Err etb target\nC6668";
        case 6669:
            return "Err 6669\nC6669";
        case 6670:
            return "Err 6670\nC6670";
        case 6671:
            return "Stack adc 6671\nC6671";
        case 6672:
            return "Err 6672\nC6672";
        case 6673:
            return "Err 6673\nC6673";
        case 6674:
            return "Stack spi\nC6674";
        case 6675:
            return "Vvt sync position\nC6675";
        case 6676:
            return "Stack adc\nC6676";
        case 6677:
            return "Ih stack\nC6677";
        case 6678:
            return "Err 6678\nC6678";
        case 6679:
            return "Err6679\nC6679";
        case 6680:
            return "Err angle cr\nC6680";
        case 6681:
            return "Delta not positive\nC6681";
        case 6682:
            return "Runtime critical timer watchdog\nC6682";
        case 6683:
            return "Same twice\nC6683";
        case 6684:
            return "Err 6684\nC6684";
        case 6685:
            return "Err 6685\nC6685";
        case 6686:
            return "Err 6686\nC6686";
        case 6687:
            return "Firing length\nC6687";
        case 6688:
            return "Advance spark\nC6688";
        case 6689:
            return "Err 6689\nC6689";
        case 6690:
            return "Err map start assert\nC6690";
        case 6691:
            return "Err map avg offset\nC6691";
        case 6692:
            return "Err map cyl offset\nC6692";
        case 6693:
            return "Err pwm duty assert\nC6693";
        case 6694:
            return "Err zero cranking fuel\nC6694";
        case 6695:
            return "Null executor\nC6695";
        case 6696:
            return "Slow not invoked\nC6696";
        case 6697:
            return "Pwm cycle start\nC6697";
        case 6698:
            return "Err array is full\nC6698";
        case 6699:
            return "Err array remove error\nC6699";
        case 6700:
            return "Err 6700\nC6700";
        case 6701:
            return "Cj125 1\nC6701";
        case 6702:
            return "Cj125 2\nC6702";
        case 6703:
            return "Err bench param\nC6703";
        case 6704:
            return "Err both fronts required\nC6704";
        case 6705:
            return "Tle8888\nC6705";
        case 6706:
            return "Knock window\nC6706";
        case 6707:
            return "Err timer test callback not happened\nC6707";
        case 6708:
            return "Err timer test callback wrong time\nC6708";
        case 6709:
            return "Err 6709\nC6709";
        case 6710:
            return "Duty invalid\nC6710";
        case 6711:
            return "Pwm duty too high\nC6711";
        case 6712:
            return "Err pwm state assert\nC6712";
        case 6713:
            return "Err pwm callback assert\nC6713";
        case 6714:
            return "Err pwm switch assert\nC6714";
        case 6715:
            return "Err zero e0 mult\nC6715";
        case 6716:
            return "Err zero e85 mult\nC6716";
        case 6720:
            return "Invalid adc\nC6720";
        case 6721:
            return "Invalid mode setting\nC6721";
        case 6722:
            return "Runtime critical task timer overflow\nC6722";
        case 6723:
            return "No etb for idle\nC6723";
        case 6724:
            return "Err tle8888 response\nC6724";
        case 6725:
            return "Err cj125 diag\nC6725";
        case 6726:
            return "6726\nC6726";
        case 6727:
            return "Vvt mode not selected\nC6727";
        case 6728:
            return "Err 6728\nC6728";
        case 6729:
            return "Artificial misfire\nC6729";
        case 6899:
            return "Instant map decoding\nC6899";
        case 6900:
            return "Stack usage communication\nC6900";
        case 6901:
            return "Stack usage mil\nC6901";
        case 6902:
            return "6902\nC6902";
        case 6903:
            return "Stack usage status\nC6903";
        case 6904:
            return "Stack usage misc\nC6904";
        case 8000:
            return "Mmc error\nC8000";
        case 8900:
            return "Err can communication\nC8900";
        case 8901:
            return "Runtime critical watch dog seconds\nC8901";
        case 8999:
            return "Err custom gaps bad\nC8999";
        case 9000:
            return "Err trigger sync\nC9000";
        case 9001:
            return "Trigger waveform\nC9001";
        case 9002:
            return "Primary too many teeth\nC9002";
        case 9003:
            return "Primary not enough teeth\nC9003";
        case 9004:
            return "Cam too many teeth\nC9004";
        case 9005:
            return "Cam not enough teeth\nC9005";
        case 9006:
            return "Primary doubled edge\nC9006";
        case 9007:
            return "Primary bad tooth timing\nC9007";
        case 9009:
            return "Skipped spark\nC9009";
        case 9011:
            return "Re adding into execution queue\nC9011";
        case 9012:
            return "Out of order coil\nC9012";
        case 9013:
            return "Too long fuel injection\nC9013";
        case 9014:
            return "Gpio chip failed pwm\nC9014";

        default:
        	return "Unknown Error Code";
       }
}


