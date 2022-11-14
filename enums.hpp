#ifndef __ENUMS__HPP__
#define __ENUMS__HPP__

enum Team
{
    Attackers,
    Defenders
};

enum PlaneState
{
    PreLauch,
    Escorting,
    Scouting,
    Evading,
    Chasing,
    Retreating,
    Destroyed,
    Retreated,
    FlyingToTarget,
    DroppingBombs
};

enum Directions
{
    Up,
    UpLeft,
    Left,
    DownLeft,
    Down,
    DownRight,
    Right,
    UpRight,
};

enum SimulationState
{
    PreStart,
    NotDetected,
    Combat,
    TargetUnderAttack,
    DefendersWin,
    AttackersWin
};

enum Speed
{
    Slow,
    Fast
};

#endif