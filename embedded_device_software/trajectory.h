#ifndef TRAJECTORY_H
#define TRAJECTORY_H

void initTrajectoryTimerInt(void);

void setTrajectoryHandler(void);

void setTrajectoryTimer(unsigned long newFreq);

void setTrajectorySource(unsigned long src);

void setTrajectoryElement(signed long pt);

void trajectoryEnable(void);

void trajectoryDisable(void);

#endif
