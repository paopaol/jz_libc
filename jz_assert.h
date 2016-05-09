#ifndef JZ_ASSERT_H
#define JZ_ASSERT_H

#define jz_assert_return_val(val, ret) 	do{if (!(val))return (ret);}while(0)
#define jz_assert_return_void(val) 	do{if (!(val))return ;}while(0)
#define jz_assert_goto(val, _goto) do{if (!(val)) {goto _goto;}}while(0)
#endif // JZ_ASSERT_H

