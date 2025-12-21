import os
import re
from typing import List, Optional

from llmatch_messages import llmatch
from langchain_core.language_models import BaseChatModel
from langchain_core.messages import HumanMessage, SystemMessage
from langchain_llm7 import ChatLLM7

from .prompts import human_prompt, pattern, system_prompt


def transcript_insightful(
        user_input: str,
        api_key: Optional[str] = None,
        llm: Optional[BaseChatModel] = None
) -> List[str]:
    """Template callable; rename this function when templating."""
    resolved_llm = llm
    if resolved_llm is None:
        if api_key is None:
            api_key = os.getenv("LLM7_API_KEY")
        if api_key is None:
            api_key = "None"
        resolved_llm = ChatLLM7(api_key=api_key, base_url="https://api.llm7.io/v1") \
            if api_key else ChatLLM7(base_url="https://api.llm7.io/v1")

    pattern_hint = f"Output must match regex: {pattern}"
    system_content = f"{system_prompt}\n\n{pattern_hint}"
    human_content = f"{human_prompt}\n\n{pattern_hint}\n\nInput:\n{user_input}".strip()

    compiled_pattern = re.compile(pattern, re.DOTALL | re.MULTILINE)

    response = llmatch(
        llm=resolved_llm,
        messages=[
            SystemMessage(content=system_content),
            HumanMessage(content=human_content),
        ],
        pattern=compiled_pattern,
        verbose=False,
    )

    if not response.get("success"):
        error_message = response.get("error_message") or "LLM7 call failed"
        raise RuntimeError(error_message)

    return response.get("extracted_data") or []
