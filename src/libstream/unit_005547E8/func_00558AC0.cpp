typedef int s32;
struct PlaybackQueryState;
extern s32 D_0064D260;
extern s32 D_0064D25C;
extern PlaybackQueryState *D_00873560;
extern "C" void func_00578500(s32 argument);
extern "C" s32 func_00557EB8(PlaybackQueryState *state);
extern "C" void func_00578480(s32 argument);
extern "C" s32 func_00558AC0(void) {
    // The query is valid only while the shared service is enabled and has a state object.
    if (D_0064D260 == 0 || D_00873560 == 0)
        return 0;
    func_00578500(D_0064D25C);
    s32 value = func_00557EB8(D_00873560);
    func_00578480(D_0064D25C);
    return value;
}
