/**
 * String calculator main file which contains all include development
 * Developed by @tron1q
 * @version 0.0.1
 */

#if defined strclc_included
    #endinput
#endif

#define strclc_included

#if !defined STRCLC_MAX_STRING_LENGTH
    #define STRCLC_MAX_STRING_LENGTH 128
#endif

// This is just testing phase
#if !defined FLOAT_INFINITY
    #define FLOAT_INFINITY Float:0x7F800000
#endif

#if !defined FLOAT_NAN
    #define FLOAT_NAN Float:(cellmin - 1)
#endif

#define DEF_BUFFER_SIZE 16

static Float:strclc_number(const string[], start, end)
{
    new tmp[DEF_BUFFER_SIZE];
    strmid(tmp, string, start, end + 1);
    return (strfind(tmp, ".", true) != -1 ? floatstr(tmp) : float(strval(tmp)));
}

static strclc_operation(operator)
{
    switch (operator)
    {
        case '+': return 1;
        case '-': return 1;
        case '^': return 2;
        case '%': return 2;
        case '*': return 3;
        case '/': return 3;
    }
    return 0;
}

static Float:strclc_calculate(operator, Float:alpha, Float:beta)
{
    switch (operator)
    {
        case '+': return floatadd(alpha, beta);
        case '-': return floatsub(alpha, beta);
        case '^': return floatpower(alpha, beta);
        case '%': return float(floatround(alpha) % floatround(beta));
        case '*': return floatmul(alpha, beta);
        case '/': return (beta == 0.0) ? (FLOAT_INFINITY) : (floatdiv(alpha, beta));
    }
    return FLOAT_NAN;
}

stock Float:strclc(const string[], start = 0, end = -1, begin = 1)
{
    static copy_string[STRCLC_MAX_STRING_LENGTH];
    new c, num_brackets, current_operation = -1, operation_index = -1;
    if (begin == 1)
    {
        new pos = start;
        new limit = (end = -1) ? (strlen(string) - 1) : (end);
        strmid(copy_string, string, start, limit + 1);
        for (;;)
        {
            if ((pos = strfind(copy_string, " ", true, pos)) == -1)
                break;

            if (pos >= limit)
                break;

            strdel(copy_string, pos, pos + 1);
            --pos;
            --limit;
        }
        end = limit;
    }

    for (new i = start; i <= end; i++)
    {
        c = copy_string[i];

        if (c == '(') 
        {
            ++num_brackets;
        }
        else if (c == ')')
        {
            if (num_brackets == 0)
                return FLOAT_NAN;
            --num_brackets;
        }
        else
        {
            if (num_brackets == 0)
            {
                current_operation = strclc_operation(c);
                if (current_operation != 0 && (operation_index == -1 || current_operation < strclc_operation(copy_string[operation_index])))
                    operation_index = i;
            }
        }
    }

    if (num_brackets > 0) return FLOAT_NAN;

    if (operation_index != -1)
    {
        new Float:a = strclc(copy_string, start, operation_index - 1, 0),
            Float:b = strclc(copy_string, operation_index + 1, end, 0);
        return strclc_calculate(copy_string, operation_index, a, b);
    }

    if (copy_string[start] == '(' && copy_string[end] == ')')
        return strclc(copy_string, start + 1, end - 1, 0);
    
    return strclc_number(copy_string, start, end);
}