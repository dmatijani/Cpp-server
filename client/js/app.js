document.addEventListener("DOMContentLoaded", () => {
    pageButtons();
    commentForm();
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