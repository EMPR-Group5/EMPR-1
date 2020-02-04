void MoveForward(int axis);
void MoveBack(int axis);
int CheckBounds(int value);
int CheckLimits(int axis);
void Calibrate(void);
void RasterScan(int resolution, char* imageArray);
void RectScan(int resolution);
void CircleScan(int resolution);
void SetupMotors();
void UpdateMotors();
void MoveToTarget(int axis);
void SetSingleCoordinate(int axis, int value);
void SetCoordinates(int* coords);
void SetupTimer(int freq);

extern int coordinates[3];
extern int home[3];
extern int targetCoordinates[3];