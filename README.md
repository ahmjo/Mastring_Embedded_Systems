# Transcript Insightful
[![PyPI version](https://badge.fury.io/py/transcript-insightful.svg)](https://badge.fury.io/py/transcript-insightful)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT)
[![Downloads](https://static.pepy.tech/badge/transcript-insightful)](https://pepy.tech/project/transcript-insightful)
[![LinkedIn](https://img.shields.io/badge/LinkedIn-blue)](https://www.linkedin.com/in/eugene-evstafev-716669181/)


This package extracts and structures key insights from video summaries or transcripts. It takes a text input describing a video's content, such as a summary or transcript, and uses LLM7 to parse and return a structured response.

## Overview

The package provides a simple way to extract the essence of a video without watching it in full. It's ideal for educational content, technical talks, or industry discussions. The structured output includes main themes, critical points, and potential implications discussed in the video.

## Installation

```bash
pip install transcript_insightful
```

## Usage

```python
from transcript_insightful import transcript_insightful

response = transcript_insightful(
    user_input="Video summary or transcript text",
    api_key="Your LLM7 API key",
    llm="Your custom LLM instance (e.g. ChatOpenAI, ChatAnthropic, etc.)"
)
print(response)  # Output: {"themes": [...], "critical_points": [...], "implications": [...]}
```

## Parameters

- `user_input`: The text input describing the video's content.
- `llm`: An optional `BaseChatModel` instance to use. Defaults to `ChatLLM7` from `langchain_llm7`.
- `api_key`: An optional API key for LLM7. Defaults to `None`.

## Using custom LLM instances

You can safely pass your own `llm` instance if you want to use another LLM, for example:

```python
from langchain_openai import ChatOpenAI
from transcript_insightful import transcript_insightful

llm = ChatOpenAI()
response = transcript_insightful(llm=llm)
```

or for example to use the anthropic:

```python
from langchain_anthropic import ChatAnthropic
from transcript_insightful import transcript_insightful

llm = ChatAnthropic()
response = transcript_insightful(llm=llm)
```

or google:

```python
from langchain_google_genai import ChatGoogleGenerativeAI
from transcript_insightful import transcript_insightful

llm = ChatGoogleGenerativeAI()
response = transcript_insightful(llm=llm)
```

## Rate limits

The default rate limits for LLM7 free tier are sufficient for most use cases of this package. If you need higher rate limits for LLM7, you can pass your own API key via environment variable `LLM7_API_KEY` or via passing it directly like `transcript_insightful(api_key="your_api_key")`.

## Getting a free API key

You can get a free API key by registering at https://token.llm7.io/

## Issues

For any issues or feature requests, please visit https://github.com/chigwell/transcript-insightful

## Author

Eugene Evstafev (github: @chigwell)
hi@eugene.plus