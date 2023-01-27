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

static bool _leep_mute = false;

#    define LEEP_PLAY_SONG(sng, force) \
        if (!_leep_mute || force) {    \
            set_tempo(sng##_tempo);    \
            PLAY_SONG(sng##_song);     \
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
DEFINE_SONG(leep_dud, SONG(LEEP_DUD));

// We define our own start-up song (instead of using STARTUP_SONG)
// so we can add logic that disables the custom startup song.
#    define SNG_STARTUP LEEP_PLAY_SONG(zelda_discovery_song, false)
#    define SNG_RESET LEEP_PLAY_SONG(mario_game_over_song, false)
#    define SNG_MUTE LEEP_PLAY_SONG(mario_lost_a_life_song, false)
#    define SNG_UNMUTE LEEP_PLAY_SONG(mario_1_up_song, false)
#    define SNG_REC_START LEEP_PLAY_SONG(rec_start_song, true)
#    define SNG_REC_1_END LEEP_PLAY_SONG(zelda_item_found_song, false)
#    define SNG_REC_2_END LEEP_PLAY_SONG(mario_1_up_song, false)
#    define SNG_REC_1_PLAY LEEP_PLAY_SONG(zelda_spirit_orb_song, false)
#    define SNG_REC_2_PLAY LEEP_PLAY_SONG(zelda_discovery_song, false)
#    define SNG_EYE_START LEEP_PLAY_SONG(mario_uw_1, false)
#    define SNG_EYE_END LEEP_PLAY_SONG(mario_uw_2, false)
#    define SNG_LOW_BEEP LEEP_PLAY_SONG(leep_low_beep, true)
#    define SNG_DUD LEEP_PLAY_SONG(leep_dud, true)

static bool _mute_just_colored = false;

static void mute_sound(bool pressed, bool with_sound) {
    if (!pressed) {
        return;
    }

    _mute_just_colored = true;
    if (_leep_mute) {
        _leep_mute = false;
        LEEP_SOLID_COLOR(GREEN, false);
        if (with_sound) {
            SNG_UNMUTE;
        }
    } else {
        if (with_sound) {
            SNG_MUTE;
        }
        LEEP_SOLID_COLOR(ORANGE, false);
        _leep_mute = true;
    }
}

void Mute_handled(keyrecord_t* record) {
    // Unpressing mute key
    if (!record->event.pressed) {
        return;
    }

    if (_mute_just_colored) {
        LEEP_LAYER_COLOR(LR_BASE, false);
        _mute_just_colored = false;
    }
}

bool IsMuted(void) { return _leep_mute; }

void LeepMute(void) { _leep_mute = true; }

void MuteWithoutSound(bool pressed) { mute_sound(pressed, false); }

void MuteWithSound(bool pressed) { mute_sound(pressed, true); }

#else  // ifdef ENABLE_LEEP_MUSIC

#    define LEEP_PLAY_SONG(sng)
#    define LEEP_PLAY_LOOP(sng)

#endif  // ifdef ENABLE_LEEP_MUSIC

#endif  // ifndef LEEP_MUSIC
