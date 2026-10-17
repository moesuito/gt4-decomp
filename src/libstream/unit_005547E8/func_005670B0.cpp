/* compiler: ee-gcc2.96-no-strict-aliasing */
typedef int s32;

struct AudioNode {
    char pad0[0x14];
    s32 dispatchEnabled;
    char pad18[0x60 - 0x18];
    void *queryState;
};

struct ControlPayload {
    s32 value0;
    s32 value1;
};

extern "C" s32 func_005657E0(void *queryState, s32 selector, s32 *mappedValue);
extern "C" s32 func_00565C30(AudioNode *self, s32 mappedValue, ControlPayload *payload,
                               s32 size, s32 operation, s32 flag);

extern "C" void func_005670B0(AudioNode *self, s32 selector,
                               s32 payloadValue0, s32 payloadValue1) {
    ControlPayload payload;
    s32 mappedValue;
    if (self->dispatchEnabled != 0 &&
        func_005657E0(self->queryState, selector, &mappedValue) == 0) {
        payload.value0 = payloadValue0;
        payload.value1 = payloadValue1;
        func_00565C30(self, mappedValue, &payload, 8, 5, 1);
    }
}
