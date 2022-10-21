#ifndef LEEP_MUSIC
#define LEEP_MUSIC

#define ENABLE_LEEP_MUSIC

#ifdef ENABLE_LEEP_MUSIC

#    define DEFINE_SONG_WITH_TEMPO(var_name, sound, tempo) \
        float   var_name##_song[][2] = sound;              \
        uint8_t var_name##_tempo     = tempo;

#    define DEFINE_SONG(var_name, sound)      \
        float   var_name##_song[][2] = sound; \
        uint8_t var_name##_tempo     = TEMPO_DEFAULT;

bool _leep_mute = false;

#    define LEEP_PLAY_SONG(sng)     \
        if (!_leep_mute) {          \
            set_tempo(sng##_tempo); \
            PLAY_SONG(sng##_song);  \
        }

#    define LEEP_PLAY_LOOP(sng)     \
        if (!_leep_mute) {          \
            set_tempo(sng##_tempo); \
            PLAY_SONG(sng##_song);  \
        }

// Explicitly only define each song once (not once per use) to limit memory usage.
// All song uses should exclusively be used through references to these
// existing songs (and not by creating new song instances).
DEFINE_SONG(zelda_discovery_song, SONG(ZELDA_DISCOVERY));
// DEFINE_SONG(zelda_guardian_battle_song, SONG(ZELDA_GUARDIAN_BATTLE));
DEFINE_SONG(zelda_item_found_song, SONG(ZELDA_ITEM_FOUND));
DEFINE_SONG(zelda_spirit_orb_song, SONG(ZELDA_SPIRIT_ORB));

DEFINE_SONG(rec_start_song, SONG(REC_START_SNG));
DEFINE_SONG(rec_end_song, SONG(REC_END_SNG));

DEFINE_SONG(mario_1_up_song, SONG(MARIO_1_UP));
DEFINE_SONG(mario_lost_a_life_song, SONG(MARIO_LOST_A_LIFE));
DEFINE_SONG_WITH_TEMPO(mario_game_over_song, SONG(MARIO_GAME_OVER), 100);

DEFINE_SONG(mario_uw_1, SONG(MARIO_UNDERWORLD_1));
DEFINE_SONG(mario_uw_2, SONG(MARIO_UNDERWORLD_2));

DEFINE_SONG(leep_low_beep, SONG(LEEP_LOW_BEEP));

// We define our own start-up song (instead of using STARTUP_SONG)
// so we can add logic that disables the custom startup song.
#    define SNG_STARTUP LEEP_PLAY_SONG(zelda_discovery_song)
#    define SNG_RESET LEEP_PLAY_SONG(mario_game_over_song)
#    define SNG_MUTE LEEP_PLAY_SONG(mario_lost_a_life_song)
#    define SNG_UNMUTE LEEP_PLAY_SONG(mario_1_up_song)
#    define SNG_REC_START LEEP_PLAY_SONG(rec_start_song)
#    define SNG_REC_1_END LEEP_PLAY_SONG(zelda_item_found_song)
#    define SNG_REC_2_END LEEP_PLAY_SONG(mario_1_up_song)
#    define SNG_REC_1_PLAY LEEP_PLAY_SONG(zelda_spirit_orb_song)
#    define SNG_REC_2_PLAY LEEP_PLAY_SONG(zelda_discovery_song)
#    define SNG_EYE_START LEEP_PLAY_SONG(mario_uw_1)
#    define SNG_EYE_END LEEP_PLAY_SONG(mario_uw_2)
#    define SNG_LOW_BEEP LEEP_PLAY_SONG(leep_low_beep)

void _mute_1(bool pressed) {
    if (!pressed) {
        return;
    }

    if (_leep_mute) {
        _leep_mute = false;
        SNG_UNMUTE;
    } else {
        SNG_MUTE;
        _leep_mute = true;
    }
}

void _mute_2(bool pressed) {
    if (!pressed) {
        return;
    }

    if (_leep_mute) {
        _leep_mute = false;
        LEEP_SOLID_COLOR(GREEN);
    } else {
        LEEP_SOLID_COLOR(ORANGE);
        _leep_mute = true;
    }
}

#else  // ifdef ENABLE_LEEP_MUSIC

#    define LEEP_PLAY_SONG(sng)
#    define LEEP_PLAY_LOOP(sng)

#endif  // ifdef ENABLE_LEEP_MUSIC

#endif  // ifndef LEEP_MUSIC
