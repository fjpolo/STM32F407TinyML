/* Generated by Edge Impulse
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef _EI_CLASSIFIER_ANOMALY_CLUSTERS_H_
#define _EI_CLASSIFIER_ANOMALY_CLUSTERS_H_

#include "edge-impulse-sdk/anomaly/anomaly.h"

// (before - mean) / scale
const float ei_classifier_anom_scale[EI_CLASSIFIER_ANOM_AXIS_SIZE] = { 1.4481135218192591, 0.36688435048906587, 2.230401280354852 };
const float ei_classifier_anom_mean[EI_CLASSIFIER_ANOM_AXIS_SIZE] = { 1.7732935408343429, 0.4489125771149618, 2.3305101185334753 };

const ei_classifier_anom_cluster_t ei_classifier_anom_clusters[EI_CLASSIFIER_ANOM_CLUSTER_COUNT] = { { { 1.7742409706115723, -0.7673258781433105, -1.0232096910476685 }, 0.3240782371631334 }
, { { 0.44636404514312744, 0.38222432136535645, 0.5436428189277649 }, 0.4165532398008927 }
, { { 1.8988462686538696, -0.32565200328826904, -1.0173760652542114 }, 0.5492992990826084 }
, { { 1.3363901376724243, -0.8428416848182678, -1.0234287977218628 }, 0.26948111040415246 }
, { { 0.19690728187561035, 0.17737874388694763, 0.19623792171478271 }, 0.2662357148544342 }
, { { 1.004578948020935, -0.8375973701477051, -1.024898886680603 }, 0.5237101017924592 }
, { { -1.1944187879562378, -1.1444909572601318, -1.0238434076309204 }, 0.8567612821150479 }
, { { 0.5200794339179993, 0.7062636613845825, 0.6427274346351624 }, 0.3567109117479875 }
, { { -0.0051923589780926704, 0.5571349859237671, 1.4976155757904053 }, 0.49090008242030003 }
, { { -0.4107855260372162, 1.203630805015564, 1.3994827270507812 }, 0.5853570464966663 }
, { { -0.5176675319671631, -0.2549598813056946, 1.0577657222747803 }, 0.5263804654687874 }
, { { 1.4358102083206177, -0.5216078758239746, -1.0237127542495728 }, 0.34234523474716766 }
, { { -0.7946917414665222, 0.05024922266602516, 0.7520042657852173 }, 0.8899690338861226 }
, { { 0.295046865940094, 1.0066107511520386, 0.473867267370224 }, 0.5718576012553644 }
, { { -0.6781907677650452, 1.1622389554977417, 0.9999663233757019 }, 0.3538432950031772 }
, { { -0.06314199417829514, 0.15628674626350403, 1.0810208320617676 }, 0.44157068907693986 }
, { { -0.4681592583656311, 0.2191208004951477, 1.244193196296692 }, 0.5063906320525042 }
, { { 0.2523763179779053, 1.6389069557189941, 0.7821446061134338 }, 0.5853340892595476 }
, { { -0.4963339567184448, 2.195633888244629, 1.545426607131958 }, 0.5107693463374952 }
, { { -0.7264044284820557, 1.4715869426727295, 1.1749119758605957 }, 0.42355699138372094 }
, { { -0.5703403353691101, 1.8616690635681152, 1.1615784168243408 }, 0.42759229864102777 }
, { { -0.43609002232551575, 1.6344475746154785, 1.6544108390808105 }, 0.4971554903249954 }
, { { 0.9277965426445007, 0.14736737310886383, 0.5065357089042664 }, 0.5064583069996936 }
, { { -0.0719369575381279, 1.1796116828918457, 1.7665836811065674 }, 0.47188661193488946 }
, { { 0.5553919076919556, -0.07133519649505615, 0.4027492105960846 }, 0.35962657007026766 }
, { { 0.18503524363040924, 0.662874162197113, 0.28289517760276794 }, 0.3463268865972573 }
, { { 1.0828524827957153, 0.554658830165863, 1.1226236820220947 }, 0.45622815346781515 }
, { { 0.7571210861206055, 1.1475591659545898, 0.8397712111473083 }, 0.6128465519173943 }
, { { -0.7411515116691589, 1.4338750839233398, 0.6682402491569519 }, 0.36120744086652873 }
, { { -0.6829498410224915, 0.628298282623291, 0.9324047565460205 }, 0.4440672886574458 }
, { { 1.0075385570526123, 1.9646430015563965, 1.0417721271514893 }, 0.47698666300904907 }
, { { -0.25477051734924316, 0.6560308933258057, -0.05430429428815842 }, 0.399807153029008 }
};

#endif // _EI_CLASSIFIER_ANOMALY_CLUSTERS_H_
