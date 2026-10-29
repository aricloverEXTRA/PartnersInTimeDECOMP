/*
 * Credits motion step (overlay 6, 0x02077CE8-0x02077D5C).
 *
 * Advances one credits motion cell by its velocity and acceleration, then
 * counts the cell down. The last frame snaps the cell onto its target and
 * reports completion to the caller.
 */

#include "frontend_scene_internal.h"

int func_ov006_02077ce8(CreditsMotion *motion)
{
    motion->vx += motion->ax;
    motion->vy += motion->ay;
    motion->x += motion->vx;
    motion->y += motion->vy;
    if (--motion->frames == 0) {
        motion->x = motion->target_x;
        motion->y = motion->target_y;
        return 1;
    }
    return 0;
}
