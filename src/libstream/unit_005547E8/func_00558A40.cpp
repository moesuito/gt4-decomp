typedef int s32;
struct PlaybackQueryState;
extern s32 D_0064D260;
extern s32 D_0064D25C;
extern PlaybackQueryState *D_00873560;
extern "C" void func_00578500(s32 argument);
extern "C" float func_00557EB0(PlaybackQueryState *state);
extern "C" void func_00578480(s32 argument);
extern "C" float func_00558A40(void) {
    // The query is valid only while the shared service is enabled and has a state object.
    if (D_0064D260 == 0 || D_00873560 == 0)
        return 0.0f;
    func_00578500(D_0064D25C);
    float value = func_00557EB0(D_00873560);
    func_00578480(D_0064D25C);
    return value;
}
