# include "../wrappers.h"

int main(void) {
	char *buf, *p;
	char arg1[MAXLINE], arg2[MAXLINE], content[MAXLINE];
	int n1 = 0, n2 = 0;

	// argument extraction
	if ((buf = getenv("QUERY_STRING")) != NULL) {
		p = strchr(buf, '&');
		*p = '\0';
		strcpy(arg1, buf);
		strcpy(arg2, p+1);
		n1 = atoi(arg1);
		n2 = atoi(arg2);
	}

	// response body
	sprintf(content, "QUERY_STRING=%s\r\n", buf);
	sprintf(content, "%sThe answer is %d + %d = %d\r\n<p>", content, n1, n2, n1+n2);

	// HTTP response
	printf("Connection: close\r\n");
	printf("Content-length: %d\r\n", (int)strlen(content));
	printf("Content-type: text/html\r\n\r\n");
	printf("%s", content);
	fflush(stdout);

	exit(0);
}
