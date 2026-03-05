#ifndef PARTICLE_H
#define PARTICLE_H

#include <stdint.h>

/*
 * ParticleType identifies what exists in one simulation cell.
 */
typedef enum {
    PARTICLE_EMPTY = 0,
    PARTICLE_SAND,
    PARTICLE_WATER,
    PARTICLE_STONE,
    PARTICLE_FIRE,
    PARTICLE_SMOKE,
    PARTICLE_WOOD,
    PARTICLE_ACID
} ParticleType;

/*
 * Particle stores the data for exactly one grid cell.
 */
typedef struct {
    ParticleType type;
    uint8_t colorVariant;  // 0-15 for color variation
    uint8_t lifetime;      // For fire/smoke
} Particle;

/*
 * createParticle builds a particle value from a type.
 */
Particle createParticle(ParticleType type);

/*
 * createEmptyParticle returns a particle in the empty state.
 */
Particle createEmptyParticle(void);

/*
 * getColorVariant returns a random color variant for a particle.
 */
uint8_t getColorVariant(void);

#endif
