#include "particle.h"

#include <stdlib.h>

/*
 * createParticle builds a particle value from a type.
 */
Particle createParticle(ParticleType type) {
    Particle particle;
    particle.type = type;
    particle.colorVariant = getColorVariant();
    particle.lifetime = 0;
    
    // Set initial lifetime for fire and smoke
    if (type == PARTICLE_FIRE) {
        particle.lifetime = 30 + (rand() % 30);
    } else if (type == PARTICLE_SMOKE) {
        particle.lifetime = 60 + (rand() % 60);
    }
    
    return particle;
}

/*
 * createEmptyParticle returns a particle in the empty state.
 */
Particle createEmptyParticle(void) {
    Particle particle;
    particle.type = PARTICLE_EMPTY;
    particle.colorVariant = 0;
    particle.lifetime = 0;
    return particle;
}

/*
 * getColorVariant returns a random color variant for a particle.
 */
uint8_t getColorVariant(void) {
    return (uint8_t)(rand() % 16);
}
