[bits 32]

global myswitch ; (ctx** old,ctx* new) old=[esp+4] new=[esp+8]
myswitch:
push eax
push ebx
push ecx
push edx
push edi
push esi
push ebp

mov eax,[esp+4+4*7] ; old
mov edx,[esp+8+4*7] ; new

mov [eax],esp
mov esp,edx

pop ebp
pop esi
pop edi
pop edx
pop ecx
pop ebx
pop eax
ret