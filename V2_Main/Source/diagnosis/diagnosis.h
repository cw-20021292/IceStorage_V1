#ifndef __DIAGNOSIS_H__
#define __DIAGNOSIS_H__

#include "prj_type.h"


void StartDiagnosis(void);
void StopDiagnosis(void);
U8 IsStartDiagnosis(void);

void SetCompleteDiagnosis(void);
void ClearCompleteDiagnosis(void);
U8 GetCompleteDiagnosis(void);


void SetDiagnosisProgress(U16 mu16TotalTime, U16 mu16CurrentTime );
U8 GetDiagnosisProgress(void);

void ControlDiagnosis(void);

#endif  /* __DIAGNOSIS_H__ */
