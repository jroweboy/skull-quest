
void reset_actors() {
    for (i = 0; i < ACTOR_NUMBER; ++i) {
        actors.state[i] = INACTIVE;
        actors.x[i] = 255;
        actors.y[i] = 240;
        actors.xDir[i] = LEFT;
        actors.yDir[i] = UP;
        actors.width[i] = NULL;
        actors.height[i] = NULL;
        actors.bbox_x[i] = NULL;
        actors.bbox_y[i] = NULL;
        actors.xSpeed[i] = NULL;
        actors.ySpeed[i] = NULL;
        actors.xRemain[i] = NULL;
        actors.yRemain[i] = NULL;
        actors.xVelocity[i] = NULL;
        actors.yVelocity[i] = NULL;
        actors.minSpeed[i] = NULL;
        actors.maxSpeed[i] = NULL;
        actors.counter[i] = NULL;
        actors.animation_delay[i] = NULL;
        actors.current_frame[i] = NULL;
        actors.type[i] = NULL;
    }
}