document.addEventListener("DOMContentLoaded", () => {
    pageButtons();
    commentForm();
    authorInput();
});

function pageButtons() {
    const prevPageButton = document.getElementById("prevPageButton");
    if (prevPageButton != undefined) {
        prevPageButton.addEventListener("click", () => {
            if (getPage() <= 0) {
                return;
            }
            changePage(getPage() - 1);
        });
    }

    const nextPageButton = document.getElementById("nextPageButton");
    if (nextPageButton != undefined) {
        nextPageButton.addEventListener("click", () => {
            changePage(getPage() + 1);
        });
    }

    const pageCount = document.getElementById("pageCount");
    if (pageCount != undefined) {
        pageCount.innerHTML = `Stranica ${getPage()}`;
    }
}

function commentForm() {
    const commentForm = document.getElementById("commentForm");
    if (commentForm == undefined) {
        return;
    }

    commentForm.addEventListener("submit", async (event) => {
        event.preventDefault();
        const formData = new URLSearchParams(new FormData(commentForm));

        const urlParams = new URLSearchParams(window.location.search);
        if (!urlParams.has("id")) {
            return;
        }

        const response = await fetch(`/dodajKomentar?id=${urlParams.get("id")}`, {
            method: commentForm.method,
            headers: {
                "Content-Type": "application/x-www-form-urlencoded"
            },
            body: formData.toString()
        });

        if (response.ok) {
            window.location.href = window.location.href;
        }
    });
}

function authorInput() {
    const authorInput = document.getElementById("autorInput");
    if (authorInput == undefined) {
        return;
    }

    let savedAuthor = localStorage.getItem("autor");
    if (savedAuthor != null) {
        authorInput.setAttribute("value", savedAuthor);
    }

    authorInput.addEventListener("input", (event) => {
        localStorage.setItem("autor", event.target.value);
    });
}

function getPage() {
    const urlParams = new URLSearchParams(window.location.search);
    if (!urlParams.has("page")) {
        return 0;
    }
    let pageNumber = Number(urlParams.get("page"));
    if (isNaN(pageNumber)) {
        return 0;
    }
    return pageNumber;
}

function changePage(page) {
    const url = new URL(window.location.href);
    const params = url.searchParams;
    params.set("page", page);
    url.search = params.toString();

    window.location.href = url.toString();
}