#ifndef __ENUMS__HPP__
#define __ENUMS__HPP__

enum Team
{
    Attackers,
    Defenders
};

enum PlaneState
{
    Escorting,
    Scouting,
    Evading,
    Chasing,
    Retreating,
    Destroyed,
    Retreated
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

#endif