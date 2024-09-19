/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hosokawa <hosokawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:45:31 by hosokawa          #+#    #+#             */
/*   Updated: 2024/09/19 11:30:42 by hosokawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "myshell.h"

t_node_info	*make_node(void)
{
	t_node_info	*node;

	node = (t_node_info *)malloc(sizeof(t_node_info));
	node->kind = -1;
	node->next = NULL;
	node->node_token = NULL;
	node->redirects = NULL;
	node->targetfd = -1;
	node->filename = NULL;
	node->filefd = -1;
	node->stashed_targetfd = -1;
	return (node);
}

t_token_info	*ft_tokendup(t_token_info *token)
{
	t_token_info	*cp_token;

	cp_token = (t_token_info *)malloc(sizeof(t_token_info));
	cp_token->word = ft_strdup(token->word);
	cp_token->kind = token->kind;
	cp_token->next = NULL;
	return (cp_token);
}

void	token_append_tail(t_node_info *node, t_token_info *cp_token)
{
	t_token_info	*search_token;

	search_token = node->node_token;
	while (search_token->next != NULL)
		search_token = search_token->next;
	search_token->next = cp_token;
}

void	word_node(t_node_info *node, t_token_info *token)
{
	t_token_info	*cp_token;

	cp_token = ft_tokendup(token);
	if (node->node_token == NULL)
		node->node_token = cp_token;
	else
		token_append_tail(node, cp_token);
}

void	eof_node(t_node_info *node)
{
	t_token_info	*eof_token;

	eof_token = (t_token_info *)ft_calloc(1, sizeof(t_token_info));
	eof_token->word = NULL;
	eof_token->kind = ROF;
	eof_token->next = NULL;
	if (node->node_token == NULL)
		node->node_token = eof_token;
	else
		token_append_tail(node, eof_token);
}

int	type_redirect_op(t_token_info *token)
{
	if (strcmp(token->word,">")==0)
		return (1);
	else if (strcmp(token->word,"<")==0)
		return (2);
	return (0);
}

//必ずひとつ目のredirectは存在しているとする
void	redirect_append_tail(t_node_info *node, t_node_info *append_redirect)
{
	t_node_info	*search_redirect;

	search_redirect = node->redirects;
	while (search_redirect->redirects != NULL)
		search_redirect = search_redirect->next;
	search_redirect->next = append_redirect;
}

//明日はここから！！！
t_token_info	*output_redirect_node(t_node_info *node, t_token_info *token)
{
	t_node_info	*redirect_node;

	redirect_node = make_node();
	redirect_node->kind=ND_REDIR_OUT;
	token = token->next;
	if (token->kind != WORD)
	{
		printf("parse_syntax_error_need_word\n");
		return (token);
	}
	redirect_node->node_token = ft_tokendup(token);
	if (node->redirects== NULL)
		node->redirects = redirect_node;
	else
		redirect_append_tail(node, redirect_node);
	return (token);
}

//ここでエラー処理もできちゃく（いいね
t_token_info	*redirect_node(t_node_info *node, t_token_info *token)
{
	t_token_info	*now_token;

	now_token=token;

	//ここではどのリダイレクトか
	if (type_redirect_op(token) == 1)
		now_token = output_redirect_node(node, token);
	// else if(type_redirect_op(token)==2)
	//	now_token=input_redirect_node(node,token);
	return (now_token);
}

t_token_info	*op_node(t_node_info *node, t_token_info *token)
{
	t_token_info	*now_token;

	//リダイレクトかどうか
	now_token=token;
	if (type_redirect_op(token) != 0)
		now_token = redirect_node(node, token);
	return (now_token);
}

t_token_info	*append_node(t_node_info *node, t_token_info *token)
{
	t_token_info	*now_token;

	now_token = token;
	if (token->kind == WORD)
		word_node(node, token);
	else if (token->kind == OP)
		now_token = op_node(node, token);
	//	else if(token->kind==RESERVE)
	//		reserve_node(node,token);
	else if (token->kind == ROF)
		eof_node(node);
	return (now_token);
}

t_node_info	*parser(t_token_info *token)
{
	t_node_info	*node;

	node = make_node();
	node->kind=ND_SIMPLE_CMD;

	while (token->next != NULL)
	{
		token = append_node(node, token);
		token = token->next;
	}
	append_node(node, token);
	return (node);
}
