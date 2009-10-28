#ifndef __SB_H__
#define __SB_H__

#ifdef __cplusplus
extern "C" {
#endif

/* stringbuffer structure */
struct sb {
    char *data;		    /* used to store the string */
    int size;			    /* the current allocated size */
    int size_written;	    /*  the written size */
};

/* snprintf for a sb , gorws sb if needed */
int sb_write(struct sb *sb, char *fmt, ...);


/* create a new stringbuffer */
struct sb *sb_new(int initial_size);

/* free the stringbuffer */
void sb_free(struct sb *sb);

#ifdef __cplusplus
}
#endif

#endif
