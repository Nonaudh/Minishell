#include "../inc/minishell.h"

volatile int sig_flag = 0;

void	sigint_handler(int signal)
{
	if (signal == SIGINT)
	{
		sig_flag = 1;
		rl_on_new_line();
    	rl_replace_line("", 0);

		rl_replace_line("\nB4SH : ", 0);
		rl_redisplay();
	}	
}

void	set_signal_action(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(struct sigaction));
	sa.sa_handler = &sigint_handler;
	sigaction(SIGINT, &sa, NULL);
}

int main(int argc, char **argv, char **env)
{
	char	*line;

	set_signal_action();
	while (1)
	{
		line = readline("B4SH : ");
		printf("%s\n", line);
		free(line);
		if (sig_flag)
		{
			printf("SIG\n");
			sig_flag = 0;
		}
	}
}