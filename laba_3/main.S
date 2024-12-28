	.syntax	unified		/* Means language is ARM UAL */
	.arch 	armv7e-m	

# ROM text code section
	.text				/* Tells AS to assemble region */
	.thumb				/* Means ARM ISA is Thumb */
	
	.global	_start		/* .global exposes symbol */
					    /* _start label is the beginning  */
                        /* ...of the program region */

	.type 	_start, %function	/* Specifies _start is a function */
_start:	
	@b 	main

	.thumb_func
	.global	main
	.type 	main, %function
main:
	@ Write code here

	@ Пример вызова функции calculate_differences
	LDR r0, =array        @ Адрес массива
	MOVS r1, #5            @ Количество элементов
	BL calculate_differences @ Вызов функции

stop:
	b 	stop

# Реализация функции calculate_differences
	.thumb_func
	.global	calculate_differences
	.type	calculate_differences, %function
calculate_differences:
	@PUSH {r2, r3, lr}      @ Сохраняем регистры
	CMP r1, #2             @ Проверяем, есть ли хотя бы два элемента
	BLT done               @ Если меньше двух элементов, завершаем

	SUBS r1, r1, #1         @ Уменьшаем количество элементов на 1 (n-1)
loop:
	LDR r2, [r0], #4       @ Загружаем текущий элемент массива (и сдвигаем указатель)
	LDR r3, [r0]           @ Загружаем следующий элемент массива
	SUB r2, r3, r2         @ Вычисляем разницу: следующий - текущий
	STR r2, [r0, #-4]      @ Записываем разницу на место текущего элемента
	SUBS r1, r1, #1        @ Уменьшаем счетчик
	BNE loop               @ Если есть ещё элементы, повторяем

done:
	@ POP {r2, r3, lr}       @ Восстанавливаем регистры
	BX lr                  @ Возврат из функции

# ROM const data section
	.section .rodata
	
	.type 	str1, %object @ mark symbol as object (metadata, debug info)
str1:
	.asciz	"STR"

	.type 	str2, %object
str2:
	.ascii	".END"

# RAM section
	.data
array:
	.word  10, 20, 30, 40, 50   @ Пример массива для тестирования
buffer:
	.space 16

	.end